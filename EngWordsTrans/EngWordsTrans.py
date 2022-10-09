import requests
import re
import os
import json

if __name__ == "__main__":
    # 创建数据文件夹
    if not os.path.exists('./EngWordsTransData'):
        os.mkdir('./EngWordsTransData')
    # -1.初始化词表，避免重复持久性存储
    buffer = []
    with open('.\\EngWordsTransData\\Lib.txt', 'a+', encoding='utf-8') as fp:
        fp.seek(0)
        for line in fp:
            buffer += [line[0:-1]]
    # 0.UA伪装
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.114 Safari/537.36 Edg/89.0.774.68'
    }
    while True:
        # 1.获取url
        post_url = "http://www.youdao.com/w/"
        word = input('enter a word:')
        if len(word) > 0:   # 防止非空输入
            post_url += word
            # 3.获取网页源码，解析数据得到单词信息
            page_text = requests.post(url=post_url, headers=headers).text
            # 4.1英文->中文
            if ord(word[0]) <= 255:
                # 4.1.1获得单词解释
                ex = '<div class="trans-container">[\s\S]*?</p>'
                ret_src_list = re.findall(ex, page_text, re.S)
                # 4.1.2规范化处理(如果查找到了结果)
                if len(ret_src_list) > 0:   # 目标单词存在
                    # 单条释义列表
                    sigex = '<li>.*?</li>'
                    explain_list = re.findall(sigex, ret_src_list[0], re.S)
                    # 单词和释义拼接形成词条
                    result = word
                    for ele in explain_list:
                        result += '\n\t'
                        result += ele[4:-5]
                    result += '\n\n'
                    # 显示释义
                    print(result)
                    # 持久化存储
                    if word not in buffer:
                        with open(".\\EngWordsTransData\\EngToChinLib.txt", 'at', encoding='utf-8') as fp:
                            fp.write(result)
                        # 添加入词库
                        with open('.\\EngWordsTransData\\Lib.txt', 'a', encoding='utf-8') as fp:
                            fp.write(word + '\n')
                        buffer += [word]
                elif len(ret_src_list) == 0:    # 目标单词不存在
                    print("The word does not exist!")
            # 4.2中文->英文
            elif ord(word[0]) > 255:
                # 4.2.1获得英文翻译结果
                ex= '<span class="contentTitle"><a class="search-js" href="/w/[a-zA-Z -]*?/#keyfrom=.*?">[a-zA-Z -]*?</a>'
                ret_src_list = re.findall(ex, page_text, re.S)
                # 4.2.2规范化处理
                if len(ret_src_list) > 0:
                    # 拼接形成词条
                    result = word + '\t\t'
                    for i in range(len(ret_src_list)):
                        if i == len(ret_src_list) - 1:
                            result += ret_src_list[i].split('>')[-2][:-3]
                        else:
                            result += ret_src_list[i].split('>')[-2][:-3] + ', '
                    result += '\n'
                    # 显示释义
                    print(result)
                    # 持久化存储
                    if word not in buffer:
                        with open(".\\EngWordsTransData\\ChinToEngLib.txt", 'at', encoding='utf-8') as fp:
                            fp.write(result)
                        # 添加入词库
                        with open('.\\EngWordsTransData\\Lib.txt', 'a', encoding='utf-8') as fp:
                            fp.write(word + '\n')
                        buffer += [word]
                elif len(ret_src_list) == 0:
                    print("Can't find target's English explaination!")