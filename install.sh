#!/bin/bash

# Ensure the script is running as root
if [ "$(id -u)" -ne 0 ]; then
    echo "Please run this script as root or with sudo."
    exit 1
fi

# Inform user about the installation process
echo "Installing clang..."

# Install clang
if command -v apt > /dev/null; then
    apt update && apt install -y clang
elif command -v dnf > /dev/null; then
    dnf install -y clang
elif command -v pacman > /dev/null; then
    pacman -Sy --noconfirm clang
elif command -v zypper > /dev/null; then
    zypper install -y clang
else
    echo "Unsupported package manager. Please install Clang manually."
    exit 1
fi

# Check if Clang was installed successfully
if ! command -v clang++ > /dev/null; then
    echo "Error: Clang installation failed. Please check your package manager."
    exit 1
fi

# Compile the program
echo "Compiling..."
if [ ! -f Todo.cpp ]; then
    echo "Error: Todo.cpp not found in the current directory."
    exit 1
fi

clang++ Todo.cpp -o todo -O3
if [ $? -ne 0 ]; then
    echo "Error: Compilation failed."
    exit 1
fi

# Move the compiled binary to a writable directory in PATH
echo "Installing 'todo' command..."

# Find a writable directory in PATH
for dir in $(echo $PATH | tr ':' '\n'); do
    if [ -w "$dir" ]; then
        install_dir="$dir"
        break
    fi
done

# Fall back to /usr/local/bin if no writable directory is found
if [ -z "$install_dir" ]; then
    echo "No writable directory found in PATH. Using /usr/local/bin as fallback."
    install_dir="/usr/local/bin"
fi

# Move the compiled binary
mv todo "$install_dir/todo"
if [ $? -ne 0 ]; then
    echo "Error: Failed to move the binary to $install_dir. Please check your permissions."
    exit 1
fi

# Inform the user of the installation path
echo "Binary installed to $install_dir."


# Inform the user of successful installation
#echo "Finished!"
echo "Type 'todo' for help."
