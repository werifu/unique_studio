package models

type Collection struct {//一个集合
	DB string
	Name string
}

type Scholar struct { //一个学者需要的信息
	Name       string
	Password   string
	Followings []string
	Followers  []string
	Assays     []string
}

type Paper struct {

	Name string
	Authors []string
}

type User struct {
	Name string
	Password string
	Status int //是否销号
	Followings []string
}