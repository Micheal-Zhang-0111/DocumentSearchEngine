// 获取 <a> 元素
var webDiskLink = document.getElementById("webDiskLink");
var baiduSearch = document.getElementById("baidusearch");

if(webDiskLink){
    // 网盘点击事件处理程序
    webDiskLink.addEventListener("click", function() {
        // 在这里编写要执行的 JavaScript 代码
        var WebDiskUrl = document.getElementById('webDiskLink');
        console.log("hello WebDisk");

        window.location.href = "http://192.168.198.128:1234/user/signin";
    }, false);
}

if(baiduSearch){
    // 绑定百度搜索点击事件处理程序
    baiduSearch.addEventListener("click", function() {
        // 在这里编写要执行的 JavaScript 代码
        var baiduSearchDoc = document.getElementById('searchContent');
        console.log("hello search");
        console.log(baiduSearchDoc.value);
        $.ajax({
        url: "/baidu/search",
        type: "POST",
        data: {
            "searchContent": baiduSearchDoc.value
        },
        error: function (jqXHR, textStatus, errorThrown) {
            if (textStatus == "error") {
            alert(textStatus + " : " + errorThrown);
            } else {
            alert(textStatus);
            }
        },
        success: function (body, textStatus, jqXHR) {
            var resp = JSON.parse(body);
            localStorage.setItem("token", resp.data.Token)
            localStorage.setItem("username", resp.data.Username)
            window.location.href = resp.data.Location;
            console.log(resp.data.Location);
        }
        });
    }, false);
}

//背景图片设置
var height = window.screen.height;
var dome = document.getElementById('dome');
dome.style.height = height + 'px';

var index=2;
setInterval(function(){
    dome.style.background="url(images/"+index+".jpg) no-repeat";
    index = (++index>=2)?1:index;
},6000)

//搜索框智能提示设置
var text_box=document.getElementById('Search_box');
var Search_btn = document.getElementById('Search_button');
function Search(){
    if(text_box.value!=""){
        var URL = "https://www.baidu.com/s?wd="+text_box.value;
        alert(URL)
        window.location.href=URL;
    }
    else{
        alert("啥都没输入还想搜？");
    }
}
Search_btn.onclick = function(){
    Search();//鼠标单击搜索
}
document.onkeyup=function(){
    if(text_box.value!=""){
        if(window.event.keyCode==13){
            Search();//键盘抬起按下回车搜索
        }
    }
}

//搜索
var ul_list = document.getElementById('ul_list');
var url1 = 'https://sp0.baidu.com/5a1Fazu8AA54nxGko9WTAnF6hhy/su?wd=';
var url2 = '&cb='
text_box.onkeyup=function(){
    if(this.value!=""){
        var script=document.createElement('script');
        script.src=url1+this.value+url2+'Search_Data';
        document.body.appendChild(script);
    }
}
function Search_Data(data){
    var arr=data.s;
    ul_list.innerText="";//每次插入标签之前，将之前标签清除
    for(var a = 0; a < arr.length; a++){
        var li = document.createElement('li');
        li.innerHTML=arr[a];
        ul_list.appendChild(li);
    }
}

text_box.onblur=function(){
    ul_list.style.display="none";
}
text_box.onfocus=function(){
    ul_list.style.display="block";
}
