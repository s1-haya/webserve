import example

obj = example.MyClass(10)
print(obj.get_value())  # 出力: 10

obj.set_value(20)
print(obj.get_value())  # 出力: 20

print(dir(example))
print(dir(obj))
