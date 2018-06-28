"""
This is just a script to tell me where tab characters appear so I can manually change them to spaces.
I need this functionality since I use two different IDE's, and they treat the tab key differently.
"""

import os


def find_tabs():
    files = os.listdir("game")

    for file in files:
        if file[-4:] not in [".hpp", ".cpp"]:
            continue
        lines = [line.rstrip("\n") for line in open("game/" + file)]
        for line in lines:
            for i in range(len(line)):
                if line[i] == "\t":
                    print(file, lines.index(line) + 1)


if __name__ == "__main__":
    find_tabs()
