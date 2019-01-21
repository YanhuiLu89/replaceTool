# replaceTool
此工具是按要求替换文件内容的小工具。
1、配置项解释如下（---后面是解释）
{
    "replaceFullFileMode":{"find_waht": ["不知道","上"],"replace_with": ["不懂得","up"]},
    -------改行配置，是全文把“不知道”替换成“不懂得”,把“上"替换成“up"。需要增加新的替换项，可以之间在中括号中增加，注意不同替换项之间用英文逗号隔开。find_what和replace_with要一一对应。
    "replaceWithFlagsMode":{"find_waht": ["先生","他"],"replace_with": ["老师","她"],"begReplace_flag": "长妈妈","endReplace_flag":"极好的文章"},
    -------改行配置，是全文把“长妈妈”之后，“极好的文章”之前，这段内容里面出现的“先生”,替换成“老师”，"他"替换成"她"。需要增加新的替换项，可以之间在中括号中增加，注意不同替换项之间用英文逗号隔开。find_what和replace_with要一一对应。
    "replaceLineMode":{"find_waht": ["的","人"],"replace_with": ["白勺","大人"],"replace_when": "大"},
    -------改行配置，是如果一行中出现了“大”，就把改行的“的”替换成“白勺”，"人"替换成"大人"。需要增加新的替换项，可以之间在中括号中增加，注意不同替换项之间用英文逗号隔开。find_what和replace_with要一一对应。
    "process_files":["从百草园到三味书屋.txt","顺风车.txt"]
    -------改行配置要处理的文件，多个文件名之间用英文逗号隔开，文件名用英文双引号括起来
}
2、配置项完成后运行replaceTool.exe就会按配置处理文件.
