1）只有main.cpp才是测试代码，主要功能为解析sms_aoc_cfg.xml文件内容
2）TinyXML类库代码为：tinystr.h、tinystr.cpp、tinyxml.h、tinyxml.cpp、tinyxmlerror.cpp、tinyxmlparser.cpp
3）编写代码时，只需要包含tinyxml.h头文件即可，编译时却需要把所有.cpp文件都加上，详情请看其makefile