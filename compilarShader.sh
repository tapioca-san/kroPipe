DIRECTORY="src/VKconfiguration/shaders"

if test -f "$DIRECTORY/vert.spv"; then
    rm "$DIRECTORY/vert.spv"
fi

if test -f "$DIRECTORY/frag.spv"; then
    rm "$DIRECTORY/frag.spv"
fi

if test -f "$DIRECTORY/shader.vert"; then
    glslc "$DIRECTORY/shader.vert" -o "$DIRECTORY/vert.spv"
fi

if test -f "$DIRECTORY/shader.frag"; then
    glslc "$DIRECTORY/shader.frag" -o "$DIRECTORY/frag.spv"
fi
