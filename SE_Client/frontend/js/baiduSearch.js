//清空sessionStorage
sessionStorage.clear();


// 获取 <a> 元素
var webDiskLink = document.getElementById("webDiskLink");

if(webDiskLink){
    // 网盘点击事件处理程序
    webDiskLink.addEventListener("click", function() {
        // 在这里编写要执行的 JavaScript 代码
        var WebDiskUrl = document.getElementById('webDiskLink');
        console.log("hello WebDisk");

        window.location.href = "http://192.168.198.128:1234/user/signin";
    }, false);
}

//时间戳，搜索框
var searchBtn = document.querySelector('.search-btn')
var style1 = document.createElement('style')
var searchBox = document.querySelector('.search-box')
var timeBox = document.querySelector('.timeBox')
searchBtn.addEventListener('focus', function () {
    style1.innerHTML = "main::before{ filter: blur(20px); transform: scale(1.01);}";
    document.head.appendChild(style1);
    searchBox.style.width = '440px'
})
searchBtn.addEventListener('blur', function () {
    document.head.removeChild(style1);
    searchBox.style.width = ""
})

setInterval(function () {
    var date = new Date()
    let hh = padZero(date.getHours())
    let mm = padZero(date.getMinutes())
    let ss = padZero(date.getSeconds())
    timeBox.innerText = hh + ':' + mm + ':' + ss
}, 1000)

function padZero(n) {
    return n > 9 ? n : '0' + n
}

//搜索框智能提示设置
const container = document.querySelector('.container'); // 获取container元素
const searchboxContainer = document.querySelector('.search-box');
const searchInput = document.querySelector('.search-btn');

// 为搜索框添加input事件监听器
searchInput.addEventListener('input', function() {
    let suggestionContainer = document.querySelector('.suggestionContainer');
    //如果suggestContainer不存在，则创建一个div
    if (!suggestionContainer) {
        suggestionContainer = document.createElement('div');
        suggestionContainer.classList.add('suggestionContainer');
        searchboxContainer.appendChild(suggestionContainer);
        //设置绝对定位
        suggestionContainer.style.position = 'absolute';
        suggestionContainer.style.left = '0';
        suggestionContainer.style.top = 'calc(100% + 10px)'// 确保它出现在搜索框的正下方
        // 将container的宽度设置给suggestionContainer
        suggestionContainer.style.width = '100%';
    }

    if (this.value){
        $.ajax({
            // contentType: "application/json; charset=UTF-8",
            url: "/home",
            type: "POST",
            data: {
                "searchContent": '1.'+searchInput.value
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
                // 检查搜索框的值,更新推荐关键词的内容
                suggestionContainer.innerHTML = '';//清空现有内容
                let len = -1;
                for(var a = 0; a < 5; a++){
                    const div = document.createElement('div');
                    div.textContent = resp[a];
                    div.style.cursor = 'pointer';
                    div.onclick = function() {
                        searchInput.value = this.textContent;//点击推荐词，将推荐词放入input标签中
                        suggestionContainer.style.display = 'none'; // 隐藏推荐词的div列表
                    };

                    // 为每个div标签添加键盘事件监听器
                    let currentIndex = -1; // 当前选中的推荐词索引，-1表示未选中
                    document.addEventListener('keydown', function(event) {
                        const items = suggestionContainer.querySelectorAll('div'); // 获取所有推荐词的div
                        if (items.length === 0) 
                            return; // 如果没有推荐词，直接返回
                        if (event.key === 'ArrowUp') {
                            // 上箭头键被按下
                            if (currentIndex > 0) {
                                currentIndex--; // 更新索引
                                searchInput.value = items[currentIndex].textContent;
                            } else {
                                currentIndex = items.length - 1; // 移到最后一个推荐词
                                searchInput.value = items[currentIndex].textContent;
                            }
                        } else if (event.key === 'ArrowDown') {
                            // 下箭头键被按下
                            if (currentIndex < items.length - 1) {
                                currentIndex++; // 更新索引
                                searchInput.value = items[currentIndex].textContent;
                            } else {
                                currentIndex = 0; // 回到第一个推荐词
                                searchInput.value = items[currentIndex].textContent;
                            }
                        } 
                        // else if (event.key === 'Enter') {
                        //     // 回车键被按下  
                        //     // 有bug
                        //     // 有bug
                        //     // 有bug
                        //     if(items[currentIndex]){
                        //         searchInput.value = items[currentIndex].textContent;
                        //     }
                        //     suggestionContainer.style.display = 'none'; // 隐藏推荐词的div列表
                        //     //跳转到搜索结果页面
                        //     SearchIput2newsPage();
                        // }
                        // 可选：高亮显示当前选中的推荐词
                        items.forEach((item, index) => {
                            if (index === currentIndex) {
                                item.style.backgroundColor = '#f0fff9'; // 高亮显示选中项
                                item.style.fontWeight = 'bold';
                            } else {
                                item.style.backgroundColor = ''; // 移除其他项的高亮显示
                                item.style.backgroundColor = ''; // 高亮显示选中项
                                item.style.fontWeight = 'normal';
                            }
                        });
                    });
                    suggestionContainer.appendChild(div);
                }
                // 如果搜索框不为空，则显示推荐词的div列表
                suggestionContainer.style.display = 'block';
                //设置光标位置

            }
        });
    }
    else {
        // 如果搜索框为空，则隐藏推荐词的div列表
        setTimeout(() => {
            suggestionContainer.style.display = 'none';
        }, 10);
    }
});

//添加事件监听器
document.addEventListener('keydown',function(event){
    if(event.key === 'Enter'){
        //按下ENTER键时发送POST请求
        $.ajax({
            url: "/home",
            type: "POST",
            data: {
                "searchContent": '2.'+searchInput.value
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
                sessionStorage.setItem('searchContentData',body);
                //跳转到搜索结果页面
                SearchIput2newsPage();
            }
        });
    }
});

// blur事件处理函数
searchInput.addEventListener('blur', function() {
    let suggestionContainer = document.querySelector('.suggestionContainer');
    // 使用setTimeout来延迟隐藏suggestionContainer，以允许点击事件发生
    setTimeout(() => {
        if (!searchInput.value) {
            suggestionContainer.style.display = 'none';
        }
    }, 500); // 延时200毫秒，根据需要调整
});


//跳转到搜索结果页面
function SearchIput2newsPage(){
    if(searchInput.value!=""){
        console.log("searchInput");
        $.ajax({
            // contentType: "application/json; charset=UTF-8",
            url: "/home/newsPage",
            type: "GET",
            data: {
                "searchContent": searchInput.value
            },
            error: function (jqXHR, textStatus, errorThrown) {
                if (textStatus == "error") {
                alert(textStatus + " : " + errorThrown);
                } else {
                alert(textStatus);
                }
            },
            success: function (body, textStatus, jqXHR) {
                window.location.href = "/home/newsPage";
            }
        });
    }
}

