import model
from model import format_str
import pymysql
class DB:
    def __init__(self, dbname):
        self.tables = []
        self.dbname = dbname
        self.psw = input('请输入密码:')


    def __do(self, sql):  #把执行函数都封装起来
        db = self.__connect(self.psw)
        self.__execute(db, sql)
        db.commit()
        self.__close(db)

    def __execute(self, db, sql):
        cursor = db.cursor()    #   获取db的光标
        cursor.execute(sql)

    def __connect(self,psw):
        db = pymysql.connect('localhost', 'root', psw, self.dbname)
        return db

    def __close(self,db):
        db.close()

    def __doget(self, sql, mode):       # 查询用
        db = self.__connect(self.psw)
        cursor = db.cursor()
        cursor.execute(sql)
        if mode == 'one':
            result = cursor.fetchone()
            print(result)
            return result
        elif mode == 'all':
            results = cursor.fetchall()
            print(results)
            return results
        db.commit()
        self.__close(db)

    def createtable(self, object1):
        table_name = object1.__table__
        if table_name not in self.tables:
            self.tables.append(table_name)  # 把表添加到数据库里

            # 构造sql语句：后边的[]表示把mappings里的kv对取出来每组配成一个list的元素
            sql = 'create table {} ({}{}{});'.format(table_name,
                                                    ','.join([k+' '+ v.col_type for k, v in object1.__mappings__.items()]),
                                                    object1.primk(),
                                                    object1.idx()
                                                    )
            print(sql)
            self.__do(sql)
            self.create(object1)
        else:
            print("The table %s exists!" % table_name)

    def create(self, object1):
        table_name = object1.__table__
        fields = []  # 放col
        params = []  # 放各个实例的值
        for k, v in object1.__mappings__.items():
            fields.append(k)
            params.append(getattr(object1, k, None))
        sql = 'insert {} ({}) values ({});'.format(table_name, ','.join(fields),
                                                        ','.join([format_str(x) for x in params]))
        print(sql)
        self.__do(sql)

    def update(self, object1, **items):    # kw里边放着更新的项目
        table_name = object1.__table__

        for k, v in items.items():
            if hasattr(object1, k):
                setattr(object1, k, v)
            elif k in type(object1).__mappings__.keys():      # 如果实例里边的值本来是null（没有设置)
                setattr(object1, k, v)
            else:
                raise AttributeError(r'%s数据表无%s字段' % (table_name, k))

        sql = 'update {} set {} where {};'.format(table_name,
                                                    ','.join([k+'='+ format_str(v) for k, v in items.items()]),
                                                        object1.__primary_key__+'='+format_str(getattr(object1, object1.primary_key)))
        print(sql)
        self.__do(sql)

    def delete(self, object1):
        table_name = object1.__table__

        sql = 'delete from {} where {};'.format(table_name,
                                                object1.__primary_key__+'='+format_str(getattr(object1,object1.primary_key)))
        print(sql)
        self.__do(sql)

    def deleteall(self, table):
        try:
            table.__table__
        except AttributeError:
            raise AttributeError('{}数据库无{}数据表'.format(self.dbname, table.__name__))

        sql = 'drop table %s;' % table.__table__
        print(sql)
        self.__do(sql)

    def findone(self, table, **expressions):
        kvlist = []
        for k, v in expressions.items():

            if k in table.__mappings__.keys():
                kvlist.append(str(k+'='+format_str(v)))
            else:
                raise AttributeError(r'%s数据表无%s字段' % (table.__table__, k))
        sql = 'select * from {} where {};'.format(table.__table__, ','.join(kvlist))
        print(sql)
        self.__doget(sql,'one')


    def findall(self, table, max_num, **expressions):
        kvlist = []
        for k, v in expressions.items():

            if k in table.__mappings__.keys():
                kvlist.append(str(k + '=' + format_str(v)))
            else:
                raise AttributeError(r'%s数据表无%s字段' % (table.__table__, k))
        sql = 'select * from {} where {} limit {};'.format(table.__table__, ','.join(kvlist), max_num)
        print(sql)
        self.__doget(sql, 'all')

    def where(self, table, **expression):   #   链式调用的where
        self.kvlist = []
        for k,v in expression.items():
            if k in table.__mappings__.keys():
                self.kvlist.append(str(k + '=' + format_str(v)))
            else:
                raise AttributeError(r'%s数据表无%s字段' % (table.__table__, k))
        self.table = table.__table__
        return self

    def findone_(self): #   链式调用的findone
        sql = 'select * from {} where {} limit 1;'.format(self.table, ','.join(self.kvlist))
        del self.kvlist #   临时用的类属性记得删除
        del self.table
        print(sql)
        self.__doget(sql, 'one')

    def findall_(self, max_num): #   链式调用的findall
        sql = 'select * from {} where {} limit {};'.format(self.table, ','.join(self.kvlist), max_num)
        del self.kvlist
        del self.table
        print(sql)
        self.__doget(sql, 'all')


db1 = DB('db1')
