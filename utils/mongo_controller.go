package utils
import (
	"gopkg.in/mgo.v2"
	"gopkg.in/mgo.v2/bson"
	"log"
	"scholars/models"
)

//var col = models.Collection{"DB1", "Scholars"}
//var s1 = models.Scholar{"John", "123456", []string{"Amy", "Mike"},[]string{"Mike", "F4"}, []string{"book1", "book2"}

const (
	URL = "127.0.0.1:27017"
	dbname = "dbname"
)
//
func InserScl(scl models.Scholar){
	session, err := mgo.Dial(URL)
	if err != nil {
		panic(err)
	}
	defer session.Close()
	c := session.DB(dbname).C("scholars")		//切换倒数据库的某个col集合
	err = c.Insert(scl)	//把学者结构塞进去
	if err != nil {
		log.Fatal(err)
	}
}

func InsertUser(user models.User){
	session, err := mgo.Dial(URL)
	if err != nil {
		panic(err)
	}
	defer session.Close()
	c := session.DB(dbname).C("users")		//切换倒数据库的某个col集合
	err = c.Insert(user)
	if err != nil {
		log.Fatal(err)
	}
}

//按照学者名字来寻找
func FindByName(name string, col models.Collection) models.Scholar {
	session, err := mgo.Dial(URL)
	if err != nil {
		panic(err)
	}
	defer session.Close()
	session.SetMode(mgo.Monotonic, true)
	collection := session.DB(col.DB).C(col.Name)
	result := models.Scholar{}
	err = collection.Find(bson.M{"name": name}).One(&result)
	if err != nil {
		log.Fatal(err)
	}
	return result
}
func Finduser(name string) models.User{
	session, err := mgo.Dial(URL)
	if err != nil {
		panic(err)
	}
	defer session.Close()
	session.SetMode(mgo.Monotonic, true)
	collection := session.DB(dbname).C("users")
	result := models.User{}
	err = collection.Find(bson.M{"name": name}).One(&result)
	if err != nil {
		log.Fatal(err)
	}
	return result
}

//根据论文名字寻找论文
func FindByAssay(name string) models.Paper{
	session, err := mgo.Dial(URL)
	if err != nil {
		panic(err)
	}
	defer session.Close()
	session.SetMode(mgo.Monotonic, true)
	collection := session.DB(dbname).C("scholar")
	result := models.Paper{}
	err = collection.Find(bson.M{"name": name}).One(&result)
	if err != nil {
		log.Fatal(err)
	}
	return result
}

//注销号
func deleteData(scl models.Scholar, col models.Collection) {
	session, err := mgo.Dial(URL)
	if err != nil {
		panic(err)
	}
	defer session.Close()
	collection := session.DB(col.DB).C(col.Name)
	err = collection.Remove(bson.M{"name": scl.Name})
}