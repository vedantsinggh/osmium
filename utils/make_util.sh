read -p "enter your utility: " name
mkdir $name
cd $name
touch $name.cpp
touch Makefile
cat ../template.mk >> Makefile
sed -i "s/NAME/$name/g" Makefile
