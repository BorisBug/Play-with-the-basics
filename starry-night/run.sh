clear               # Clear the terminal
mkdir -p build      # Create the build directory
rm -rf ./build/*    # Delete files in build
gcc starry-night.c ./../lib/xy-print.c -I./../lib -o ./build/out -l ncurses  # Compile
./build/out         # Run the main program
rm -rf ./build      # Delete folder