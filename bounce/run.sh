clear               # Clear the terminal
mkdir -p build      # Create the build directory
rm -rf ./build/*    # Delete files in build
gcc bounce.c -o ./build/bounce  # Compile
./build/bounce      # Run the main program