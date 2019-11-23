# 存放各种class

class Field(object):#  定义各种字段的基类,如string对应的varchar或者int对应的int类
    def __init__(self, col_type, max_len, **kwargs):
        self.col_type = col_type    # 字段类型
        self.max_len = max_len
        self.default = None
        if kwargs:# 有其他属性或者方法的话
            for k, v in kwargs.items():
                if hasattr(self, k):        # 如果有k这种属性的话
                    setattr(self, k, v)

    #打印类的type时的格式
    def __str__(self):
        return '<%s>'%(self.__class__.__name__)

class StringField(Field):
    def __init__(self, max_len, **kwargs):
        super().__init__(col_type='varchar({})'.format(max_len), max_len=max_len, **kwargs)
        # {}里边可以放format格式化过的变量
        #用Field类的初始化方法

class IntField(Field):
    def __init__(self, **kwargs):
        super().__init__(col_type='int', max_len=4, **kwargs)

class Int64Field(Field):
    def __init__(self, **kwargs):
        super().__init__(col_type='bigint', max_len=8, **kwargs)

class Index():
    def __init__(self, index_name, col, length=4):  # length默认4
        self.index_name = index_name
        self.col = col
        self.length=length


