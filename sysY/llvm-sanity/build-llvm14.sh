llvm_source_dir=/home/zhao/Disk/llvm1405
build_dir=/home/zhao/Disk/build_llvm1405
if [ -d ${build_dir} ]
then
# dangerous! rethink before uncomment #rm -rf ${build_dir}
fi
mkdir ${build_dir}

cd ${build_dir}
cmake ${llvm_source_dir}/llvm \
  -DLLVM_TARGETS_TO_BUILD="X86;ARM;RISCV" \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_THREADS=On \
  -DBUILD_SHARED_LIBS=On 

cmake_res=`echo $?`
if [ ${cmake_res} -eq 0 ]
then
make -j2
else
echo "cmake fails, exit"
fi
