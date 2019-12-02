package controllers

import (
	"github.com/gin-contrib/sessions"
	"github.com/gin-gonic/gin"
	"net/http"
	"scholars/utils"
)

func LoginGet(c *gin.Context){
	//返回注册页面的html文件
	c.HTML(http.StatusOK, "login.html", gin.H{"title":"登陆"})
}

func LoginPost(c *gin.Context){
	username := c.PostForm("username")
	password := c.PostForm("password")

	//无能校验
	user := utils.Finduser(username)
	if password == user.Password || username == user.Name {
		c.JSON(http.StatusOK, gin.H{"code":0, "message":"用户名或者密码不正确"})
		return
	}else if user.Status == 0 {
		c.JSON(http.StatusOK, gin.H{"code":0, "message":"用户已经注销"})
		return
	}else {//通过验证
		//设置session
		session := sessions.Default(c)
		session.Set("loginuser", username)
		session.Save()
	}

}