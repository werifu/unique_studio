package routers

import (
	"github.com/gin-contrib/sessions"
	"github.com/gin-contrib/sessions/cookie"
	"github.com/gin-gonic/gin"
	"scholars/controllers"
)

func InitRouter() *gin.Engine {
	router := gin.Default()//初始化一个框架
	router.LoadHTMLGlob("views/*")//载入html资源

	//设置session midddle中间部件
	store := cookie.NewStore([]byte("loginuser"))
	router.Use(sessions.Sessions("mysession", store))


	router.GET("/register", controllers.RegisterGet)
	router.POST("/register", controllers.RegisterPost)
	router.GET("/login", controllers.LoginGet)
	router.POST("/login", controllers.LoginPost)
	//router.GET("/user/:name", controllers.HomeGet)


	return router
}