#!/usr/bin/env python3

import os
import subprocess
import sys
from enum import Enum, auto
from pathlib import Path

import click
import yaml

# Main params
PROJECT_NAME = Path(__file__).resolve().parent.name.lower().replace(" ", "_")
IMAGE_NAME = "my-images/" + PROJECT_NAME + ":dev"
CONTAINER_NAME = PROJECT_NAME + "-dev"
COMMANDS_FILES = (
    Path("config/post_start_commands.yaml"),
    Path("context/post_start_commands.yaml"),
)

ENABLE_GUI = False
USE_X11 = False
PROXY_GIT = True

uid = os.getuid()
gid = os.getgid()
pwd = os.getcwd()


def get_user() -> str:
    user = os.environ.get("USER")
    if user is None:
        exit(
            "❌ Невозможно получить имя пользователя из переменных окружения. "
            "Объявите его в переменных или укажите вручную."
        )
    return user  # pyright: ignore[reportReturnType]


def run_cmd(cmd, capture_output=True, shell=False):
    """Выполняет команду и возвращает результат."""
    if capture_output:
        return subprocess.run(cmd, capture_output=True, text=True, shell=shell)
    return subprocess.run(cmd, text=True, shell=shell)


def exit(msg: str):
    """Выход из программы с сообщением."""
    click.echo(msg)
    sys.exit(1)


def get_home_path() -> str:
    home_path = run_cmd(
        ["docker", "exec", CONTAINER_NAME, "sh", "-c", "echo $HOME"],
    ).stdout.strip()
    return home_path


class ContainerStatus(Enum):
    """Статусы контейнера."""

    STOP = auto()
    UP = auto()
    NONE = auto()


def check_container_status() -> ContainerStatus:
    """Проверка статуса контейнера."""
    result = run_cmd(
        [
            "docker",
            "ps",
            "-a",
            "--filter",
            f"name={CONTAINER_NAME}",
            "--format",
            "{{.Status}}",
        ],
    )
    if not result.stdout:
        return ContainerStatus.NONE
    if "Up" in result.stdout:
        return ContainerStatus.UP
    return ContainerStatus.STOP


def remove_image() -> bool:
    """Удаление старого образа."""
    click.echo(f"Проверяем наличие старого образа '{IMAGE_NAME}'...")
    result = run_cmd(["docker", "images", "-q", IMAGE_NAME], capture_output=True)

    if result.stdout.strip():
        click.echo(f"Удаляем старый образ '{IMAGE_NAME}'...")
        del_result = run_cmd(["docker", "rmi", "-f", IMAGE_NAME], capture_output=True)
        if del_result.returncode == 0:
            click.echo(f"✅ Старый образ '{IMAGE_NAME}' удалён.")
        else:
            click.echo(
                f"⚠️ Не удалось удалить старый образ: {del_result.stderr.strip()}"
            )
            return False
    else:
        click.echo(f"✅ Старый образ '{IMAGE_NAME}' не найден. Продолжаем...")

    return True


def copy_to_container(from_path: Path, to_path: str):
    result = run_cmd(
        [
            "docker",
            "cp",
            f"{from_path}",
            f"{CONTAINER_NAME}:{to_path}",
        ],
        capture_output=False,
    )
    if result.returncode != 0:
        exit(f"❌ Ошибка {from_path} в контейнер {CONTAINER_NAME}:{to_path}.")


def load_commands_from_yaml(path: Path) -> list[str]:
    """Загружает список команд из YAML-файла."""
    if not path.exists():
        click.echo(f"⚠️ Файл {path} не найден.")
        return []
    try:
        with open(path, encoding="utf-8") as f:
            data = yaml.safe_load(f)
        commands = data.get("post_start_commands", [])
        if not isinstance(commands, list):
            click.echo(
                f"⚠️ Поле 'post_start_commands' в {path} должно быть списком. Пропускаем."
            )
            return []
        return commands
    except Exception as e:
        click.echo(f"❌ Ошибка при чтении {path}: {e}")
        return []


def execute_post_start_commands():
    """Выполняет команды после запуска контейнера."""
    commands: list[str] = []
    for commands_yaml in COMMANDS_FILES:
        commands.extend(load_commands_from_yaml(commands_yaml))

    if not commands:
        return
    click.echo("Выполняем post-start команды...\n⎯⎯⎯⎯⎯⎯⎯")
    for cmd in commands:
        if cmd is None:
            click.echo("Команда не указана. Проверьте файл с командами.")
            continue

        click.echo(f"\033[0;34m{cmd}\033[0m")

        result = run_cmd(
            f"docker exec -t {CONTAINER_NAME} bash -c '{cmd} && echo ⎯⎯⎯⎯⎯⎯⎯'",
            shell=True,
            capture_output=False,
        )
        if result.returncode != 0:
            click.echo(f"⚠️ Ошибка при выполнении команды '{cmd}'")


class AliasedGroup(click.Group):
    """Класс для создания alias для команд."""

    def get_command(self, ctx, cmd_name):
        rv = click.Group.get_command(self, ctx, cmd_name)
        if rv is not None:
            return rv

        aliases = {
            "b": "build",
            "u": "up",
            "a": "attach",
            "d": "down",
            "s": "status",
        }
        if cmd_name in aliases:
            return click.Group.get_command(self, ctx, aliases[cmd_name])
        return None


@click.group(cls=AliasedGroup)
def cli():
    """CLI для управления Docker-контейнером."""
    pass


@cli.command()
@click.option("--no-cache", is_flag=True, help="Сборка без использования кэша.")
def build(no_cache: bool):
    """Сборка Docker-образа."""
    if check_container_status() != ContainerStatus.NONE:
        exit(
            f"❌ Контейнер на базе образа '{IMAGE_NAME}' уже существует. Остановите и удалите его командой 'down/d'."
        )

    if not remove_image():
        sys.exit(1)

    cmd = [
        "docker",
        "build",
        "-t",
        IMAGE_NAME,
        "--build-arg",
        f"PROJECT_NAME={PROJECT_NAME}",
        "--build-arg",
        f"NEW_USER={get_user()}",
        # HACK pass here args for build
        ".",
    ]

    if no_cache:
        cmd.insert(-1, "--no-cache")
        click.echo("Сборка без использования кэша.")

    click.echo(f"Сборка образа '{IMAGE_NAME}'.")
    result = run_cmd(cmd, capture_output=False)

    if result.returncode == 0:
        click.echo(f"✅ Образ '{IMAGE_NAME}' успешно собран.")
    else:
        exit(f"❌ Ошибка при сборке образа '{IMAGE_NAME}'.")


@cli.command()
def up():
    """Запуск контейнера из образа."""
    status = check_container_status()

    if status == ContainerStatus.UP:
        click.echo(f"✅ Контейнер {CONTAINER_NAME} уже запущен.")
        return
    elif status == ContainerStatus.STOP:
        click.echo(f"Контейнер {CONTAINER_NAME} остановлен. Запускаем...")
        run_cmd(["docker", "start", CONTAINER_NAME])
        click.echo(f"✅ Контейнер {CONTAINER_NAME} запущен.")
    elif status == ContainerStatus.NONE:
        click.echo(f"Контейнер {CONTAINER_NAME} не найден. Запускаем новый...")
        cmd = [
            "docker",
            "run",
            "-d",
            "-v",
            f"{os.getcwd()}:/{PROJECT_NAME}",
            "--name",
            CONTAINER_NAME,
            "-it",
            "-u",
            f"{uid}:{gid}",
        ]

        if PROXY_GIT:
            cmd.extend(
                [
                    "-v",
                    f"{os.getenv('SSH_AUTH_SOCK')}:{os.getenv('SSH_AUTH_SOCK')}",
                    "-e",
                    f"SSH_AUTH_SOCK={os.getenv('SSH_AUTH_SOCK')}",
                ]
            )
        if ENABLE_GUI:
            cmd.extend(
                [
                    "-v",
                    "/tmp/.X11-unix:/tmp/.X11-unix",
                    "-v",
                    "/dev/dri:/dev/dri",
                    "-e",
                    f"DISPLAY={os.getenv('DISPLAY')}",
                    "--ipc=host",
                ]
            )

        cmd.extend(
            [
                IMAGE_NAME,
                "/bin/bash",
            ]
        )
        result = run_cmd(
            cmd,
            capture_output=False,
        )
        if result.returncode != 0:
            exit(f"❌ Ошибка при запуске контейнера {CONTAINER_NAME}.")

        if PROXY_GIT:
            home_path = get_home_path()
            copy_to_container(Path.home() / ".gitconfig", f"{home_path}/.gitconfig")
            copy_to_container(Path.home() / ".ssh", f"{home_path}")

        execute_post_start_commands()

        click.echo(f"✅ Контейнер {CONTAINER_NAME} запущен.")
    else:
        assert False, "unknown container status"
    if ENABLE_GUI and USE_X11:
        run_cmd(["xhost", "+local:docker"])


@cli.command()
def attach():
    """Подключение к контейнеру."""
    result = run_cmd(
        [
            "docker",
            "ps",
            "--filter",
            f"name={CONTAINER_NAME}",
            "--format",
            "{{.Names}}",
        ],
    )
    if CONTAINER_NAME in result.stdout:
        click.echo(f"Подключение к контейнеру {CONTAINER_NAME}...")
        run_cmd(
            ["docker", "exec", "-it", CONTAINER_NAME, "bash"],
            capture_output=False,
        )
    else:
        exit(f"❌ Контейнер {CONTAINER_NAME} не запущен.")


@cli.command()
def down():
    """Остановка и удаление контейнеров."""
    result = run_cmd(
        [
            "docker",
            "ps",
            "-a",
            "--filter",
            f"name={CONTAINER_NAME}",
            "--format",
            "{{.Names}}",
        ],
    )
    if CONTAINER_NAME in result.stdout:
        click.echo(f"Останавливаем контейнер {CONTAINER_NAME}...")
        run_cmd(["docker", "stop", CONTAINER_NAME])
        click.echo(f"Удаляем контейнер {CONTAINER_NAME}...")
        run_cmd(["docker", "rm", CONTAINER_NAME])
        if ENABLE_GUI and USE_X11:
            run_cmd(["xhost", "-local:docker"])
        click.echo(f"✅ Контейнер {CONTAINER_NAME} остановлен и удалён.")
    else:
        click.echo(f"⚠️ Контейнер {CONTAINER_NAME} не найден.")


@cli.command()
def status():
    """Проверка статуса контейнера."""
    result = run_cmd(
        [
            "docker",
            "ps",
            "-a",
            "--filter",
            f"name={CONTAINER_NAME}",
            "--format",
            "{{.Status}}",
        ],
    )

    click.echo(f"📦 Контейнер: {CONTAINER_NAME}")
    if result.stdout:
        if "Up" in result.stdout:
            click.echo(f"✅ Статус: {result.stdout.strip()}")
        else:
            click.echo(f"⚠️ Статус: {result.stdout.strip()}")
    else:
        click.echo("❌  Статус: Не существует.")  # тут нужно два пробела!


if __name__ == "__main__":
    cli()
