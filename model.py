from field import Field,StringField, IntField, Int64Field, Index

# 定义每个Table类型继承的Model类型的元类
# 实际上每个model应该继承于dict类型(二维表本身就是k,v对
class ModelMetaclass(type):
    def __new__(cls, name, bases, attrs):
        #   解释：name是类名, bases是Table继承的类的tuple, attrs是属性or方法
        if __name__ == 'Model':
            return type.__new__(cls, name, bases, attrs)    # 用type动态创建一个model的实例
        #   下边针对建立table相关类
        mappings = dict()   # mappings用来存放table中的k,v对
        index = dict()
        primary_key = None
        for k, v in attrs.items():
            if isinstance(v, Field):
                mappings[k] = v
            elif k == 'primary_key':
                primary_key = v
            elif k == 'index':
                index['index_name'] = v.index_name
                index['col'] = v.col
                index['length'] = v.length

        for k in mappings.keys():      # 把table需要的kv对从attrs中删除
            attrs.pop(k)
        attrs['__mappings__'] = mappings         # 保留属性和col的映射关系
        attrs['__table__'] = name      # 保留table的名字（也是类的名字）
        attrs['__primary_key__'] = primary_key
        attrs['__index__'] = index
        return type.__new__(cls, name, bases, attrs)


class Model(dict, metaclass=ModelMetaclass):
    def __init__(self, **kw):
        super(Model, self).__init__(**kw)

    def __getattr__(self, item):        # 获取属性
        try:
            return self[item]
        except:
            raise AttributeError(r"'Model'类无%s属性"% item)

    def __setattr__(self, key, value):      # 增加属性
        self[key] = value

    def format_str(self, arg):      # 格式化
        if type(arg).__name__ == 'str':
            return '"%s"' % arg
        else:
            return str(arg)

    def primk(self):             # 用于设置主键
        if self.primary_key:
            try:
                #   尝试从__mappings__的属性列表中寻找设置的primk名字，如果没有会报错
                self.__mappings__[self.primary_key]
                return ',primary key (%s)' % self.primary_key
            except:
                raise AttributeError(r"主键错误：数据表%s中无%s字段"% (self.__table__, self.primary_key))
        else:
            return ''
    def idx(self):      # 设置索引
        try:
            self.index
            return ', index {} ({})'.format(self.__index__['index_name'],
                                            self.__index__['col'] + '(%s)' % str(self.__index__['length']))
        except:
            return ''

def format_str(arg):        # 在转化为SQL语言时的格式化
    if arg is None:     # None和null的转化
        return 'null'
    elif type(arg).__name__ == 'str':
        return '"%s"' % arg
    else:
        return str(arg)


class User(Model):
    users = []
    id = IntField()
    price = IntField()
    name = StringField(max_len=30)
    info = StringField(max_len=100)
    #index = Index('name_idx', 'name', length=4)
    primary_key = 'id'

if __name__ == '__main__':
    pass
