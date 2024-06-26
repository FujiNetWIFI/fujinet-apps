for IMAGE in $(ls -1 *.gif)
do
    echo \*\*\* CONVERTING $IMAGE
    NAME="${IMAGE%.*}"
    convert $IMAGE[0] -grayscale Rec709Luminance -depth 2 $NAME.gray
    mv $NAME.gray $NAME.gfx
done

