# **Linux**

## **Installation**

- Install GNU Make:
  - `sudo apt install make -y`
- Install the Allegro Library ([Wiki Link](https://github.com/liballeg/allegro_wiki/wiki/Quickstart)):
  - For Debian-based distributions:
    - `sudo add-apt-repository ppa:allegro/5.2`
    - `sudo apt-get install liballegro*5.2 liballegro*5-dev`
    - `sudo apt-get install liballegro-ttf5-dev`

## **Compilation and Execution**

- **Comment out** the following line in the Makefile:
  ```
  ...
  # Descomente no Windows!
  # ALLEGRO = allegro32 (or allegro64)
  ...
  ```
- Run `make`

---

# **Windows**

## **Installation**

- Install GNU Make ([Link Here](https://www.gnu.org/software/make/))
  - Pay attention to the architecture of the downloaded executable (32-bit or 64-bit).
- Download the Allegro Library ([Link Here](https://liballeg.org/download.html#windows))
  - The installed Allegro library must match the same architecture (32-bit or 64-bit) as GNU Make.
- **Remember** to add both the `make` binary and the Allegro binaries (`\lib` and `\bin`) to your `PATH`, otherwise, they won't be accessible.

## **Compilation and Execution**

- **Uncomment** the following line in the Makefile, and make sure to match the architecture (32-bit or 64-bit):
  ```
    ...
    # Descomente no Windows!
    ALLEGRO = allegro32 (or allegro64)
    ...
  ```
- Run `make`
