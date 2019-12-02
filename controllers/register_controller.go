package controllers

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"scholars/models"
	"scholars/utils"
)
func regicheck(c *gin.Context) bool {
	if utils.Finduser(c.PostForm("username")).Name != ""{
		c.JSON(http.StatusOK, gin.H{"code":0, "message":"用户已经存在"})
		return false
	}else if c.PostForm("password") != c.PostForm("repassword"){
		c.JSON(http.StatusOK, gin.H{"code":0, "message":"二次密码不同"})
		return false
	}

	return true
}
func RegisterGet(c *gin.Context){
	//返回注册页面的html文件
	c.HTML(http.StatusOK, "register.html", gin.H{"title":"注册"})
}

func RegisterPost(c *gin.Context){
	//先获取表单信息

	if regicheck(c) {
		username := c.PostForm("username")
		password := c.PostForm("password")
		user := models.User{username, password, 0, []string{}}
		//fmt.Println(username, password, repassword)
		//注册用户名和密码
		utils.InsertUser(user)
		c.Redirect(http.StatusOK, "/login")
	}else {
		return
	}
}

