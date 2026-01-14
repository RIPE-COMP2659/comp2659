# Python Prototype

## Overview
The code contained in this directory is intended to function as a prototype for the game, determining the overall structure in a faster environment than developing it in C.

## Setup
If you're not already using [pyenv](https://github.com/pyenv/pyenv) for your Python version management, this is a very useful tool but optional to setup. There's a Windows fork in their documentation.

See the [official documentation](https://packaging.python.org/en/latest/guides/installing-using-pip-and-virtual-environments/) for troubleshooting.

Parker is using Python 3.14.0a1-win32, but other versions may work.

<details>
  <summary>Windows</summary>

  **Create your virtual environment:**
  ```bash
  python -m venv .venv
  ```

  **Activate it**:
  ```bash
  .venv\Scripts\activate
  ```

  **Install dependencies**:
  ```bash
  python -m pip install -r requirements.txt
  ```
</details>

<details>
  <summary>Unix/macOS</summary>

  Note: Untested

  **Create your virtual environment:**
  ```bash
  python3 -m venv .venv
  ```

  **Activate it:**
  ```bash
  source .venv/bin/activate
  ```

  **Install dependencies:**
  ```bash
  python3 -m pip install -r requirements.txt
  ```
</details>