import pandas as pd

# 读取csv文件
data = pd.read_csv('matlab.csv', header=None)

# 打印并写入data.c
with open('data.c', 'w', encoding='utf-8') as f:
    for col in range(data.shape[1]):
        arr = data.iloc[:, col].tolist()
        arr_str = ', '.join(f'{x:.6f}f' for x in arr)
        c_array = f'float col{col+1}[{len(arr)}] = {{{arr_str}}};\n'
        print(c_array)
        f.write(c_array)
