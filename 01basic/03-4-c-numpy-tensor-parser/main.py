import numpy as np

def dtype2int(dtype: np.dtype):
    # 将numpy的dtype转换为整数标识
    if dtype == np.float32:
        return 0
    elif dtype == np.float16:
        return 1
    elif dtype == np.float64:
        return 2
    elif dtype == np.int32:
        return 3
    elif dtype == np.uint32:
        return 4
    elif dtype == np.int64:
        return 5
    # 如果传入了不支持的数据类型，则断言失败
    assert False, f"Unsupport dtype {dtype}"


def int2dtype(itype: int):
    # 将整数标识转换回numpy的dtype
    if itype == 0:
        return np.float32
    elif itype == 1:
        return np.float16
    elif itype == 2:
        return np.float64
    elif itype == 3:
        return np.int32
    elif itype == 4:
        return np.uint32
    elif itype == 5:
        return np.int64
    
    assert False, f"Unsupport itype {itype}"

def load_tensor(file):
    with open(file, "rb") as f:
        binary_data = f.read()
    
    # 读取文件头部信息: 魔数、维度数量、数据类型
    magic_number, ndims, dtype = np.frombuffer(binary_data, np.uint32, count=3, offset=0)
    assert magic_number == 0xFCCFE2E2, f"{file} 不是一个张量文件。"

    # 读取张量形状
    dims = np.frombuffer(binary_data, np.uint32, count=ndims, offset=3*4)
    np_dtype = int2dtype(dtype)
    # 读取张良数据, 并根据形状重新构造张量
    return np.frombuffer(binary_data, np_dtype, offset=(ndims+3)*4).reshape(*dims)

def save_tensor(file, tensor: np.ndarray):
    with open(file, 'rb') as f:
        binary_data = f.read()
    
    # 读取文件头部信息：魔数、维度数量、数据类型
    magic_number, ndims, dtype = np.frombuffer(binary_data, np.uint32, count=3, offset=0)
    assert magic_number == 0xFCCFE2E2, f"{file} 不是一个张量文件。"
    
    dims = np.frombuffer(binary_data, np.uint32, count=ndims, offset=3*4)    
    np_dtype = int2dtype(dtype)
    return np.frombuffer(binary_data, np_dtype, offset=(ndims + 3)*4).reshape(*dims)

def save_tensor(file, tensor: np.ndarray):
    # 将张量保存到文件
    with open(file, 'wb') as f:
        f.write(np.array([0xFCCFE2E2, tensor.ndim, dtype2int(tensor.dtype)] + list(tensor.shape), dtype=np.uint32).tobytes())
        f.write(tensor.tobytes())

# 测试代码, 创建一个张量
tensor = np.arange(50).reshape(5, 2, 5)
# 将张量保存到文件
save_tensor("./workspace/test.tensor", tensor)

loaded = load_tensor("./workspace/test.tensor")
print(loaded, loaded.shape, loaded.dtype, sep='\n')

# 验证保存和加载后的张量是否相同
print(np.allclose(tensor, loaded))




