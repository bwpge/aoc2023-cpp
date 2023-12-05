#!/usr/bin/env python

import argparse, shutil, sys
from pathlib import Path


MAIN_CPP_TEMPLATE = """#include "aoc/aoc.hpp"

#include <iostream>

void part1() {{
    std::cout << "Hello from Part 1!\\n";
}}

void part2() {{
    std::cout << "Hello from Part 2!\\n";
}}

int main() {{
    std::cout << "Day {day} solution:";
    part1();
    part2();

    return 0;
}}
"""

HEADER_TEMPLATE = "#pragma once\n"

TEST_CPP_TEMPLATE = """#include "aoc/aoc.hpp"

#include <gtest/gtest.h>

TEST(Day{day}, HelloWorld) {{
    ASSERT_TRUE(true);
}}
"""

README_TEMPLATE = """# Day {day}{title}

## Part One

## Part Two
"""


def bail(msg: str, code=1):
    print(f"error: {msg}")
    sys.exit(code)


def confirm(prompt: str | None = None):
    if prompt is None:
        prompt = "Are you sure you want to continue?"
    choice = input(f"{prompt} [y/N] ").lower()
    return choice == "y" or choice == "yes"


def info(msg: str):
    print(f"  - {msg}")


parser = argparse.ArgumentParser(
    description="Generate an Advent of Code project directory for a given day",
)
parser.add_argument(
    "-n",
    "--num",
    metavar="DAY",
    dest="day",
    type=int,
    required=True,
    help="the day number for this project",
)
parser.add_argument(
    "-d",
    "--parent-dir",
    metavar="DIR",
    type=str,
    help="optionally specify a different parent directory for the project",
)
parser.add_argument(
    "-t",
    "--title",
    type=str,
    help="optional title for the day's problem (used in README template)",
)


def validate_parent(path: Path):
    if not path.exists() or not path.is_dir():
        bail(f"parent directory `{path.resolve()}` does not exist")

    expected_files = [path.joinpath("CMakeLists.txt"), path.joinpath(".gitignore")]
    for f in expected_files:
        if not f.exists():
            bail(
                "parent directory is not suitable\n"
                f"  - expected to find `{f.name}` in `{f.parent.resolve()}`"
            )


if __name__ == "__main__":
    args = parser.parse_args()

    parent_dir = Path(args.parent_dir or "..")
    validate_parent(parent_dir)

    day = args.day
    proj_name = f"day{day}"
    proj_title = args.title or ""
    proj_dir = parent_dir.joinpath(proj_name)
    proj_include = proj_dir.joinpath("include")

    if proj_dir.exists():
        if proj_dir.is_file():
            bail("destination path exists and is not a directory")
        print(
            f"WARNING: the destination directory `{proj_dir.resolve()}` already exists"
        )
        print("The ENTIRE DIRECTORY will be deleted and cannot be recovered!\n")
        if not confirm():
            sys.exit(0)

    print(f"\nCreating solution at: {proj_dir.resolve()}\n")

    if proj_dir.exists():
        info("Deleting existing directory")
        shutil.rmtree(proj_dir)

    info("Creating directory structure")
    proj_dir.mkdir()
    proj_include.mkdir()

    info("Writing files")
    with open(proj_dir.joinpath("main.cpp"), "w") as f:
        f.write(MAIN_CPP_TEMPLATE.format(day=day))

    with open(proj_include.joinpath(f"{proj_name}.hpp"), "w") as f:
        f.write(HEADER_TEMPLATE)

    with open(proj_dir.joinpath(f"{proj_name}_test.cpp"), "w") as f:
        f.write(TEST_CPP_TEMPLATE.format(day=day))

    with open(proj_dir.joinpath("README.md"), "w") as f:
        title = ""
        if proj_title:
            title = f": {proj_title}"
        f.write(README_TEMPLATE.format(day=day, title=title))

    print("\nProject successfully created!")
    print(
        f"\nBe sure to add `{proj_name}` to `solutions` "
        "in CMakeLists.txt and add a link in the README.\n"
    )
