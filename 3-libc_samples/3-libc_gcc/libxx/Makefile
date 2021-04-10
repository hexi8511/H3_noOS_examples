SUB_DIR:=libc libm libx



make_dir=$(foreach var,$(SUB_DIR), make -C $(var);)

all: 
	echo "================"
	echo  "make lib start"
	$(make_dir)
clean:
	echo "================"
	echo  "clean lib start"
	$(make_dir)
	
	
