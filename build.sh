build_mode="shared"
build_type="release"
read -p "Change default build options (Y/N): " choice

choice=$(echo "$choice" | tr '[:upper:]' '[:lower:]')
if [ "$choice" == "y" ]; then
    read -p "Build mode (1 for static, 2 for shared): " option1
    if [ "$option1" == "1" ]; then
        build_mode="static"
    elif [ "$option1" == "2" ]; then
        build_mode="shared"
    else
        echo "Invalid option for build mode. Using default."
    fi

    read -p "Build type (1 for debug, 2 for release): " option2
    if [ "$option2" == "1" ]; then
        build_type="debug"
    elif [ "$option2" == "2" ]; then
        build_type="release"
    else
        echo "Invalid option for build type. Using default."
    fi
else
    echo "Using default options..."
fi

echo "Build mode is $build_mode"
echo "Build type is $build_type"
scons build_mode="$build_mode" build_type="$build_type"
