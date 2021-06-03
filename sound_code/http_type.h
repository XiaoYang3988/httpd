#ifndef __http_type
#define __http_type



/* http请求头每行最大值 */
#define HTTP_REQUEST_HEAD_LINE_MAX 1024

/*
** 请求行长度
*/
/* 版本字符串最大字符长度 */
#define VERSIONS_MAX 16
/* 请求url最大字符长度 */
#define REQUEST_URL_MAX 128
/* 请求最大字符长度 */
#define METHOD_MAX 32

/*
** 通用头长度
*/
/* 时间 最大字符长度 */
#define DATE_MAX 64
/* http连接管理 最大字符长度 */
#define CONNECTION_MAX 64
/* MIME类型 最大字符长度 */
#define MIME_VERSION_MAX 32
/* 分块编码 最大字符长度 */
#define TRAILER_MAX 32
/* http编码方式 最大字符长度 */
#define TRANSFER_ENCODING_MAX 32
/* 发送端想要升级使用的新版本或协议 最大字符长度 */
#define UPDATE_MAX 16
/* 显示报文经过的中间节点(代理、网关) 最大字符长度 */
#define VIA_MAX 128
/* 缓存控制 最大字符长度 */
#define CACHE_CONTROL_MAX 32
/* 只用于客户端发送的请求中。客户端会要求所有的中间服务器不返回缓存的资源。最大字符长度 */
#define PRAGMA_MAX 32

/*
** 除通用头与实体头以外的请求首部长度
*/
/* sec_ch_ua */
#define SEC_CH_UA_MAX 260
/* sec_ch_ua_mobile */
#define SEC_CH_UA_MOBILE 128
/* sec_ch_ua_platform */
#define SEC_CH_UA_PLATFORM 128
/* upgrade_insecure_requests */
#define UPGRADE_INSECURE_REQUESTS 16
/* sec_fetch_site */
#define SEC_FETCH_SITE 32
/* sec_fetch_mode */
#define SEC_FETCH_MODE 32
/* sec_fetch_user */
#define SEC_FETCH_USER 16
/* sec_fetch_dest */
#define SEC_FETCH_DEST 32

/* 客户端ip 最大字符长度 */
#define CLIENT_IP_MAX 128
/* 客户端用户的邮箱地址 最大字符长度 */
#define FROM_MAX 64
/* 接受请求的服务器主机名和端口号 最大字符长度 */
#define HOST_MAX 144
/* 当前url的文档的url 最大字符长度 */
#define REFERER_MAX 128
/* 客户端显示器显示颜色的信息 最大字符长度 */
#define UA_COLOR_MAX 64
/* 客户端cpu类型和制造商 最大字符长度 */
#define UA_CPU_MAX 64
/* 客户端操作系统类型和版本 最大字符长度 */
#define UA_OS_MAX 128
/* 客户端显示器的像素信息 最大字符长度 */
#define UA_PIXELS_MAX 64
/* 客户端发起请求的应用程序名称 最大字符长度 */
#define USER_AGENT_MAX 260

/* 要求服务器发送的媒体类型 最大字符长度 */
#define ACCEPT_MAX 256
/* 要求服务器发送的字符集 最大字符长度 */
#define ACCEPT_CHARSET_MAX 64
/* 要求服务器发送的编码方式 最大字符长度 */
#define ACCEPT_ENCODING_MAX 64
/* 要求服务器能发送的语言类型 最大字符长度 */
#define ACCEPT_LANGUAGE_MAX 64
/* 要求服务器可以使用那些扩展传输编码 最大字符长度 */
#define TE_MAX 64

/* 允许客户端列出某请求所要求的服务器行为 最大字符长度 */
#define EXPECT_MAX 128
/* 如果实体标记与文档当前的实体标记相匹配，就获取这份文档 最大字符长度 */
#define IF_MATCH_MAX 128
/* 除非在某个指定的日期之后资源被修改过，否则就限制这个请求 最大字符长度 */
#define IF_MODIFIED_SINCE_MAX 128
/* 如果提供的实体标记与当前文档的实体标记不相符，就获取文档 最大字符长度 */
#define IF_NONE_MATCH_MAX 128
/* 允许对文档的某个范围进行条件请求 最大字符长度 */
#define IF_RANGE_MAX 128
/* 除非在某个指定日期之后资源没有被修过过，否则就限制这个请求 最大字符长度 */
#define IF_UNMODIFIED_SINCE_MAX 128
/* 如果服务器支持范围请求，就请求资源的指定范围 最大字符长度 */ 
#define RANGE_MAX 128

/* 包含了客户端提供给服务器，以便对其自身进行认证的数据 最大字符长度 */
#define AUTHORIZATION_MAX 256
/* 客户端用它向服务器传送一个令牌，它并不是真正的安全首部，但确实隐含了安全功能 最大字符长度 */
#define COOKIE_MAX 4096
/* 用来说明请求端支持的cookie版本 最大字符长度 */
#define COOKIE_2_MAX 64

/* 在通往源端服务器的路径上，将请求转发给其他代理或网关的最大次数，与TRACE方法一同使用 最大字符长度 */
#define MAX_FORWARD_MAX 64
/* 与authorization首部相同，但这个首部是在与代理进行认证时使用的 最大字符长度 */
#define PROXY_AUTHORIZATION_MAX 64
/* 与connection首部相同，但这个首部实在与代理建立连接时使用的 最大字符长度 */
#define PROXY_CONNECTION_MAX 64

/*
** 响应类型长度
*/
/* 状态代码的文本描述 最大字符长度 */
#define REASON_PHRASE_MAX 16


/*
** 通用实体首部
*/
/* 列出了可以对此实体实行的请求方法 最大字符长度 */
#define ALLOW_MAX 64
/* 告知客户端实体实际上位与何处，用于将接受端定向到资源的位置上 最大字符长度 */
#define LOCATION_MAX 128
/* 解析主题中的相对url时使用的基础url 最大字符长度 */
#define CONTENT_BASE_MAX 128
/* 对内容主体执行的任何编码方式 最大字符长度 */
#define CONTENT_ENCODING_MAX 32
/* 内容主体使用的语言 最大字符长度 */
#define CONTENT_LANGUAGE_MAX 32
/* 内容主体的长度 最大字符长度 */
#define CONTENT_LENGTH_MAX 32
/* 资源实际所在的位置 最大字符长度 */
#define CONTENT_LOCATION_MAX 128
/* MD5校验和 最大字符长度 */
#define CONTENT_MD5_MAX 64
/* 在整个资源中此实体表示的字节范围 最大字符长度 */
#define CONTENT_RANGE_MAX 64
/* 这个主题的对象类型 最大字符长度 */
#define CONTENT_TYPE 32
/* 与此实体相关的实体标记 最大字符长度 */
#define ETAG_MAX 64
/* 实体不再有效，要从原始的源端再次获取此实体的日期和时间 最大字符长度 */
#define EXPIRES_MAX 64
/* 这个实体最后一次被修改的日期和时间 最大字符长度 */
#define LAST_MODIFIED_MAX 64

/*
** 可接受实体最大的长度的字节数
*/
#define HTTP_ENTITY_MAX 0xa00000

#endif