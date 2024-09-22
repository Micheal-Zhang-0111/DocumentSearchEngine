document.addEventListener('DOMContentLoaded', (event) => {
    const newsData = JSON.parse(sessionStorage.getItem('searchContentData'));
    // var newsData = sessionStorage.getItem('searchContentData');
    console.log(newsData);
    
    //将新闻div添加到其中的容器元素
    const cardContainer = document.getElementById("cardBox");
    //遍历newsData数组
    newsData.forEach(news => {
        //为每个新闻创建一个div元素
        const newsDiv = document.createElement("div");
        //添加类名用于css设计
        newsDiv.classList.add("card");
    
        // 创建并添加h2元素用于显示新闻标题
        const titleElement = document.createElement("h2");
        titleElement.innerText = news["title"];
        newsDiv.appendChild(titleElement);
    
        //创建并添加h4元素用于显示新闻原网页链接
        const sourceLinkElement = document.createElement("a");
        sourceLinkElement.innerText = "原网站链接";
        sourceLinkElement.setAttribute("href", news["link"]);
        sourceLinkElement.setAttribute("target", "_blank");
        newsDiv.appendChild(sourceLinkElement);

        // 创建并添加p元素用于显示新闻内容
        const keywordsElement = document.createElement("p");
        keywordsElement.innerText = news["keywords"];
        newsDiv.appendChild(keywordsElement);

        // 将新闻div添加到容器元素中
        cardContainer.appendChild(newsDiv);
    });

});


//鼠标点击出现爱心特效  这部分特效是从一个网页搬过来的  觉得挺好看的
(function (window, document, undefined) {
    var hearts = [];
    window.requestAnimationFrame = (function () {
        return window.requestAnimationFrame ||
        window.webkitRequestAnimationFrame ||
        window.mozRequestAnimationFrame ||
        window.oRequestAnimationFrame ||
        window.msRequestAnimationFrame ||
        function (callback) {
            setTimeout(callback, 1000 / 60);
        }
    })();
    init();
    function init() {
        css(".heart{width: 10px;height: 10px;position: fixed;background: #f00;transform: rotate(45deg);-webkit-transform: rotate(45deg);-moz-transform: rotate(45deg);}.heart:after,.heart:before{content: '';width: inherit;height: inherit;background: inherit;border-radius: 50%;-webkit-border-radius: 50%;-moz-border-radius: 50%;position: absolute;}.heart:after{top: -5px;}.heart:before{left: -5px;}");
        attachEvent();
        gameloop();
    }
    function gameloop() {
        for (var i = 0; i < hearts.length; i++) {
            if (hearts[i].alpha <= 0) {
                document.body.removeChild(hearts[i].el);
                hearts.splice(i, 1);
                continue;
            }
            hearts[i].y--;
            hearts[i].scale += 0.004;
            hearts[i].alpha -= 0.013;
            hearts[i].el.style.cssText = "left:" + hearts[i].x + "px;top:" + hearts[i].y + "px;opacity:" + hearts[i].alpha + ";transform:scale(" + hearts[i].scale + "," + hearts[i].scale + ") rotate(45deg);background:" + hearts[i].color;
        }
        requestAnimationFrame(gameloop);
    }
    function attachEvent() {
        var old = typeof window.onclick === "function" && window.onclick;
        window.onclick = function (event) {
            old && old();
            createHeart(event);
        }
    }
    function createHeart(event) {
        var d = document.createElement("div");
        d.className = "heart";
        hearts.push({
            el: d,
            x: event.clientX - 5,
            y: event.clientY - 5,
            scale: 1,
            alpha: 1,
            color: randomColor()
        });
        document.body.appendChild(d);
    }
    function css(css) {
        var style = document.createElement("style");
        style.type = "text/css";
        try {
            style.appendChild(document.createTextNode(css));
        } catch (ex) {
            style.styleSheet.cssText = css;
        }
        document.getElementsByTagName('head')[0].appendChild(style);
    }
    function randomColor() {
        return "rgb(" + (~~(Math.random() * 255)) + "," + (~~(Math.random() * 255)) + "," + (~~(Math.random() * 255)) + ")";
    }
})(window, document);