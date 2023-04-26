Search.setIndex({docnames:["api_documentation","getting_started","index","python/ctrlutils","python/ctrlutils.RedisClient","python/ctrlutils.Timer","python/ctrlutils.decode_matlab","python/ctrlutils.eigen","python/ctrlutils.eigen.Affine3d","python/ctrlutils.eigen.AngleAxisd","python/ctrlutils.eigen.Isometry3d","python/ctrlutils.eigen.LdltMatrixXd","python/ctrlutils.eigen.Projective3d","python/ctrlutils.eigen.Quaterniond","python/ctrlutils.eigen.Translation3d","python/ctrlutils.far_quaternion","python/ctrlutils.lookat_error","python/ctrlutils.near_quaternion","python/ctrlutils.orientation_error","python/ctrlutils.ostream_redirect","python/ctrlutils.pd_control","python/ctrlutils.pseudo_inverse"],envversion:{"sphinx.domains.c":2,"sphinx.domains.changeset":1,"sphinx.domains.citation":1,"sphinx.domains.cpp":4,"sphinx.domains.index":1,"sphinx.domains.javascript":2,"sphinx.domains.math":2,"sphinx.domains.python":3,"sphinx.domains.rst":2,"sphinx.domains.std":2,"sphinx.ext.intersphinx":1,sphinx:56},filenames:["api_documentation.rst","getting_started.rst","index.rst","python/ctrlutils.rst","python/ctrlutils.RedisClient.rst","python/ctrlutils.Timer.rst","python/ctrlutils.decode_matlab.rst","python/ctrlutils.eigen.rst","python/ctrlutils.eigen.Affine3d.rst","python/ctrlutils.eigen.AngleAxisd.rst","python/ctrlutils.eigen.Isometry3d.rst","python/ctrlutils.eigen.LdltMatrixXd.rst","python/ctrlutils.eigen.Projective3d.rst","python/ctrlutils.eigen.Quaterniond.rst","python/ctrlutils.eigen.Translation3d.rst","python/ctrlutils.far_quaternion.rst","python/ctrlutils.lookat_error.rst","python/ctrlutils.near_quaternion.rst","python/ctrlutils.orientation_error.rst","python/ctrlutils.ostream_redirect.rst","python/ctrlutils.pd_control.rst","python/ctrlutils.pseudo_inverse.rst"],objects:{"":[[3,0,0,"-","ctrlutils"]],"ctrlutils.RedisClient":[[4,2,1,"","__init__"],[4,2,1,"","acl_cat"],[4,2,1,"","acl_deluser"],[4,2,1,"","acl_dryrun"],[4,2,1,"","acl_genpass"],[4,2,1,"","acl_getuser"],[4,2,1,"","acl_help"],[4,2,1,"","acl_list"],[4,2,1,"","acl_load"],[4,2,1,"","acl_log"],[4,2,1,"","acl_log_reset"],[4,2,1,"","acl_save"],[4,2,1,"","acl_setuser"],[4,2,1,"","acl_users"],[4,2,1,"","acl_whoami"],[4,2,1,"","append"],[4,2,1,"","auth"],[4,2,1,"","bf"],[4,2,1,"","bgrewriteaof"],[4,2,1,"","bgsave"],[4,2,1,"","bitcount"],[4,2,1,"","bitfield"],[4,2,1,"","bitfield_ro"],[4,2,1,"","bitop"],[4,2,1,"","bitpos"],[4,2,1,"","blmove"],[4,2,1,"","blmpop"],[4,2,1,"","blpop"],[4,2,1,"","brpop"],[4,2,1,"","brpoplpush"],[4,2,1,"","bzmpop"],[4,2,1,"","bzpopmax"],[4,2,1,"","bzpopmin"],[4,2,1,"","cf"],[4,2,1,"","client_getname"],[4,2,1,"","client_getredir"],[4,2,1,"","client_id"],[4,2,1,"","client_info"],[4,2,1,"","client_kill"],[4,2,1,"","client_kill_filter"],[4,2,1,"","client_list"],[4,2,1,"","client_no_evict"],[4,2,1,"","client_pause"],[4,2,1,"","client_reply"],[4,2,1,"","client_setname"],[4,2,1,"","client_tracking"],[4,2,1,"","client_tracking_off"],[4,2,1,"","client_tracking_on"],[4,2,1,"","client_trackinginfo"],[4,2,1,"","client_unblock"],[4,2,1,"","client_unpause"],[4,2,1,"","cms"],[4,2,1,"","command"],[4,2,1,"","command_docs"],[4,2,1,"","command_getkeysandflags"],[4,2,1,"","command_list"],[4,2,1,"","config_get"],[4,2,1,"","config_resetstat"],[4,2,1,"","config_rewrite"],[4,2,1,"","config_set"],[4,2,1,"","copy"],[4,2,1,"","dbsize"],[4,2,1,"","debug_object"],[4,2,1,"","decr"],[4,2,1,"","decrby"],[4,2,1,"","delete"],[4,2,1,"","dump"],[4,2,1,"","echo"],[4,2,1,"","eval"],[4,2,1,"","eval_ro"],[4,2,1,"","evalsha"],[4,2,1,"","evalsha_ro"],[4,2,1,"","execute_command"],[4,2,1,"","exists"],[4,2,1,"","expire"],[4,2,1,"","expireat"],[4,2,1,"","expiretime"],[4,2,1,"","failover"],[4,2,1,"","fcall"],[4,2,1,"","fcall_ro"],[4,2,1,"","flushall"],[4,2,1,"","flushdb"],[4,2,1,"","from_url"],[4,2,1,"","ft"],[4,2,1,"","function_delete"],[4,2,1,"","function_dump"],[4,2,1,"","function_flush"],[4,2,1,"","function_kill"],[4,2,1,"","function_list"],[4,2,1,"","function_load"],[4,2,1,"","function_restore"],[4,2,1,"","function_stats"],[4,2,1,"","geoadd"],[4,2,1,"","geodist"],[4,2,1,"","geohash"],[4,2,1,"","geopos"],[4,2,1,"","georadius"],[4,2,1,"","georadiusbymember"],[4,2,1,"","geosearch"],[4,2,1,"","geosearchstore"],[4,2,1,"","get"],[4,2,1,"","get_connection_kwargs"],[4,2,1,"","get_encoder"],[4,2,1,"","get_image"],[4,2,1,"","get_matrix"],[4,2,1,"","getbit"],[4,2,1,"","getdel"],[4,2,1,"","getex"],[4,2,1,"","getrange"],[4,2,1,"","getset"],[4,2,1,"","graph"],[4,2,1,"","hdel"],[4,2,1,"","hello"],[4,2,1,"","hexists"],[4,2,1,"","hget"],[4,2,1,"","hgetall"],[4,2,1,"","hincrby"],[4,2,1,"","hincrbyfloat"],[4,2,1,"","hkeys"],[4,2,1,"","hlen"],[4,2,1,"","hmget"],[4,2,1,"","hmset"],[4,2,1,"","hrandfield"],[4,2,1,"","hscan"],[4,2,1,"","hscan_iter"],[4,2,1,"","hset"],[4,2,1,"","hsetnx"],[4,2,1,"","hstrlen"],[4,2,1,"","hvals"],[4,2,1,"","incr"],[4,2,1,"","incrby"],[4,2,1,"","incrbyfloat"],[4,2,1,"","info"],[4,2,1,"","json"],[4,2,1,"","keys"],[4,2,1,"","lastsave"],[4,2,1,"","latency_doctor"],[4,2,1,"","latency_graph"],[4,2,1,"","latency_histogram"],[4,2,1,"","latency_history"],[4,2,1,"","latency_latest"],[4,2,1,"","latency_reset"],[4,2,1,"","lcs"],[4,2,1,"","lindex"],[4,2,1,"","linsert"],[4,2,1,"","llen"],[4,2,1,"","lmove"],[4,2,1,"","lmpop"],[4,2,1,"","load_external_module"],[4,2,1,"","lock"],[4,2,1,"","lolwut"],[4,2,1,"","lpop"],[4,2,1,"","lpos"],[4,2,1,"","lpush"],[4,2,1,"","lpushx"],[4,2,1,"","lrange"],[4,2,1,"","lrem"],[4,2,1,"","lset"],[4,2,1,"","ltrim"],[4,2,1,"","memory_malloc_stats"],[4,2,1,"","memory_purge"],[4,2,1,"","memory_stats"],[4,2,1,"","memory_usage"],[4,2,1,"","mget"],[4,2,1,"","migrate"],[4,2,1,"","module_list"],[4,2,1,"","module_load"],[4,2,1,"","module_loadex"],[4,2,1,"","module_unload"],[4,2,1,"","move"],[4,2,1,"","mset"],[4,2,1,"","msetnx"],[4,2,1,"","object"],[4,2,1,"","parse_response"],[4,2,1,"","persist"],[4,2,1,"","pexpire"],[4,2,1,"","pexpireat"],[4,2,1,"","pexpiretime"],[4,2,1,"","pfadd"],[4,2,1,"","pfcount"],[4,2,1,"","pfmerge"],[4,2,1,"","ping"],[4,2,1,"","pipeline"],[4,2,1,"","psetex"],[4,2,1,"","psync"],[4,2,1,"","pttl"],[4,2,1,"","publish"],[4,2,1,"","pubsub"],[4,2,1,"","pubsub_channels"],[4,2,1,"","pubsub_numpat"],[4,2,1,"","pubsub_numsub"],[4,2,1,"","quit"],[4,2,1,"","randomkey"],[4,2,1,"","readonly"],[4,2,1,"","readwrite"],[4,2,1,"","register_script"],[4,2,1,"","rename"],[4,2,1,"","renamenx"],[4,2,1,"","replicaof"],[4,2,1,"","reset"],[4,2,1,"","restore"],[4,2,1,"","role"],[4,2,1,"","rpop"],[4,2,1,"","rpoplpush"],[4,2,1,"","rpush"],[4,2,1,"","rpushx"],[4,2,1,"","sadd"],[4,2,1,"","save"],[4,2,1,"","scan"],[4,2,1,"","scan_iter"],[4,2,1,"","scard"],[4,2,1,"","script_exists"],[4,2,1,"","script_flush"],[4,2,1,"","script_kill"],[4,2,1,"","script_load"],[4,2,1,"","sdiff"],[4,2,1,"","sdiffstore"],[4,2,1,"","select"],[4,2,1,"","sentinel"],[4,2,1,"","sentinel_ckquorum"],[4,2,1,"","sentinel_failover"],[4,2,1,"","sentinel_flushconfig"],[4,2,1,"","sentinel_get_master_addr_by_name"],[4,2,1,"","sentinel_master"],[4,2,1,"","sentinel_masters"],[4,2,1,"","sentinel_monitor"],[4,2,1,"","sentinel_remove"],[4,2,1,"","sentinel_reset"],[4,2,1,"","sentinel_sentinels"],[4,2,1,"","sentinel_set"],[4,2,1,"","sentinel_slaves"],[4,2,1,"","set"],[4,2,1,"","set_image"],[4,2,1,"","set_matrix"],[4,2,1,"","set_response_callback"],[4,2,1,"","setbit"],[4,2,1,"","setex"],[4,2,1,"","setnx"],[4,2,1,"","setrange"],[4,2,1,"","shutdown"],[4,2,1,"","sinter"],[4,2,1,"","sintercard"],[4,2,1,"","sinterstore"],[4,2,1,"","sismember"],[4,2,1,"","slaveof"],[4,2,1,"","slowlog_get"],[4,2,1,"","slowlog_len"],[4,2,1,"","slowlog_reset"],[4,2,1,"","smembers"],[4,2,1,"","smismember"],[4,2,1,"","smove"],[4,2,1,"","sort"],[4,2,1,"","sort_ro"],[4,2,1,"","spop"],[4,2,1,"","srandmember"],[4,2,1,"","srem"],[4,2,1,"","sscan"],[4,2,1,"","sscan_iter"],[4,2,1,"","stralgo"],[4,2,1,"","strlen"],[4,2,1,"","substr"],[4,2,1,"","sunion"],[4,2,1,"","sunionstore"],[4,2,1,"","swapdb"],[4,2,1,"","sync"],[4,2,1,"","tdigest"],[4,2,1,"","time"],[4,2,1,"","topk"],[4,2,1,"","touch"],[4,2,1,"","transaction"],[4,2,1,"","ts"],[4,2,1,"","ttl"],[4,2,1,"","type"],[4,2,1,"","unlink"],[4,2,1,"","unwatch"],[4,2,1,"","wait"],[4,2,1,"","watch"],[4,2,1,"","xack"],[4,2,1,"","xadd"],[4,2,1,"","xautoclaim"],[4,2,1,"","xclaim"],[4,2,1,"","xdel"],[4,2,1,"","xgroup_create"],[4,2,1,"","xgroup_createconsumer"],[4,2,1,"","xgroup_delconsumer"],[4,2,1,"","xgroup_destroy"],[4,2,1,"","xgroup_setid"],[4,2,1,"","xinfo_consumers"],[4,2,1,"","xinfo_groups"],[4,2,1,"","xinfo_stream"],[4,2,1,"","xlen"],[4,2,1,"","xpending"],[4,2,1,"","xpending_range"],[4,2,1,"","xrange"],[4,2,1,"","xread"],[4,2,1,"","xreadgroup"],[4,2,1,"","xrevrange"],[4,2,1,"","xtrim"],[4,2,1,"","zadd"],[4,2,1,"","zcard"],[4,2,1,"","zcount"],[4,2,1,"","zdiff"],[4,2,1,"","zdiffstore"],[4,2,1,"","zincrby"],[4,2,1,"","zinter"],[4,2,1,"","zintercard"],[4,2,1,"","zinterstore"],[4,2,1,"","zlexcount"],[4,2,1,"","zmpop"],[4,2,1,"","zmscore"],[4,2,1,"","zpopmax"],[4,2,1,"","zpopmin"],[4,2,1,"","zrandmember"],[4,2,1,"","zrange"],[4,2,1,"","zrangebylex"],[4,2,1,"","zrangebyscore"],[4,2,1,"","zrangestore"],[4,2,1,"","zrank"],[4,2,1,"","zrem"],[4,2,1,"","zremrangebylex"],[4,2,1,"","zremrangebyrank"],[4,2,1,"","zremrangebyscore"],[4,2,1,"","zrevrange"],[4,2,1,"","zrevrangebylex"],[4,2,1,"","zrevrangebyscore"],[4,2,1,"","zrevrank"],[4,2,1,"","zscan"],[4,2,1,"","zscan_iter"],[4,2,1,"","zscore"],[4,2,1,"","zunion"],[4,2,1,"","zunionstore"]],"ctrlutils.Timer":[[5,2,1,"","__init__"]],"ctrlutils.eigen":[[8,1,1,"","Affine3d"],[9,1,1,"","AngleAxisd"],[10,1,1,"","Isometry3d"],[11,1,1,"","LdltMatrixXd"],[12,1,1,"","Projective3d"],[13,1,1,"","Quaterniond"],[14,1,1,"","Translation3d"]],"ctrlutils.eigen.Affine3d":[[8,2,1,"","__init__"],[8,2,1,"","identity"],[8,2,1,"","inverse"],[8,2,1,"","is_approx"],[8,2,1,"","make_affine"],[8,2,1,"","prerotate"],[8,2,1,"","rotate"],[8,2,1,"","rotation"],[8,2,1,"","set"],[8,2,1,"","set_identity"]],"ctrlutils.eigen.AngleAxisd":[[9,2,1,"","__init__"],[9,2,1,"","identity"],[9,2,1,"","inverse"],[9,2,1,"","is_approx"],[9,2,1,"","matrix"],[9,2,1,"","set"]],"ctrlutils.eigen.Isometry3d":[[10,2,1,"","__init__"],[10,2,1,"","identity"],[10,2,1,"","inverse"],[10,2,1,"","is_approx"],[10,2,1,"","make_affine"],[10,2,1,"","prerotate"],[10,2,1,"","rotate"],[10,2,1,"","rotation"],[10,2,1,"","set"],[10,2,1,"","set_identity"]],"ctrlutils.eigen.LdltMatrixXd":[[11,2,1,"","__init__"],[11,2,1,"","solve"]],"ctrlutils.eigen.Projective3d":[[12,2,1,"","__init__"],[12,2,1,"","identity"],[12,2,1,"","inverse"],[12,2,1,"","is_approx"],[12,2,1,"","make_affine"],[12,2,1,"","prerotate"],[12,2,1,"","rotate"],[12,2,1,"","rotation"],[12,2,1,"","set"],[12,2,1,"","set_identity"]],"ctrlutils.eigen.Quaterniond":[[13,2,1,"","__init__"],[13,2,1,"","angular_distance"],[13,2,1,"","conjugate"],[13,2,1,"","dot"],[13,2,1,"","from_two_vectors"],[13,2,1,"","identity"],[13,2,1,"","inverse"],[13,2,1,"","is_approx"],[13,2,1,"","matrix"],[13,2,1,"","norm"],[13,2,1,"","normalize"],[13,2,1,"","normalized"],[13,2,1,"","set"],[13,2,1,"","set_from_two_vectors"],[13,2,1,"","set_identity"],[13,2,1,"","slerp"],[13,2,1,"","squared_norm"],[13,2,1,"","unit_random"]],"ctrlutils.eigen.Translation3d":[[14,2,1,"","__init__"],[14,2,1,"","identity"],[14,2,1,"","inverse"],[14,2,1,"","is_approx"],[14,2,1,"","set"]],"ctrlutils.ostream_redirect":[[19,2,1,"","__init__"]],ctrlutils:[[4,1,1,"","RedisClient"],[5,1,1,"","Timer"],[6,3,1,"","decode_matlab"],[7,0,0,"-","eigen"],[15,3,1,"","far_quaternion"],[16,3,1,"","lookat_error"],[17,3,1,"","near_quaternion"],[18,3,1,"","orientation_error"],[19,1,1,"","ostream_redirect"],[20,3,1,"","pd_control"],[21,3,1,"","pseudo_inverse"]]},objnames:{"0":["py","module","Python module"],"1":["py","class","Python class"],"2":["py","method","Python method"],"3":["py","function","Python function"]},objtypes:{"0":"py:module","1":"py:class","2":"py:method","3":"py:function"},terms:{"0":[4,5,8,10,12,15,17,18,20,21],"1":[4,9,13,14,16,18,20],"100":4,"1000":[4,5],"12":[8,9,10,12,13,14],"127":4,"1970":4,"1e":[8,9,10,12,13,14],"2":[4,20],"256":4,"2d":20,"3":[4,8,9,10,12,13,14,15,16,17,18,20],"3d":[17,20],"4":[4,8,10,12,13,17],"4d":20,"5":4,"6":4,"6379":4,"boolean":4,"byte":4,"case":4,"catch":4,"class":[3,4,5,7,8,9,10,11,12,13,14,19],"default":4,"do":4,"final":4,"float":[4,8,9,10,12,13,14,20,21],"function":[3,4,8,9,10,12,13,14,15,17,20],"import":4,"int":4,"long":17,"new":4,"null":4,"return":[4,6,15,16,17,18,20],"static":[8,9,10,12,13,14],"throw":4,"true":[4,19],"try":4,"while":4,A:[4,21],As:4,By:4,For:[4,18],If:4,In:4,It:4,NO:4,NOT:4,No:4,ON:4,ONE:4,Of:17,That:4,The:[4,15,17],There:4,To:4,Will:4,With:4,__init__:[4,5,8,9,10,11,12,13,14,19],_id:4,_quaternionbase_quaterniond:13,_type:4,ab:18,abc:4,abl:4,abort:4,about:[4,16],absolut:4,absttl:4,acceler:20,accept:4,access:4,accord:4,acknowledg:4,acl:4,acl_cat:4,acl_delus:4,acl_dryrun:4,acl_genpass:4,acl_getus:4,acl_help:4,acl_list:4,acl_load:4,acl_log:4,acl_log_reset:4,acl_sav:4,acl_setus:4,acl_us:4,acl_whoami:4,aclfil:4,acquir:4,across:4,activ:4,actual:4,ad:4,add:4,addit:4,addition:4,addr:4,address:4,administr:4,after:4,again:4,aggreg:4,agreement:4,algo:4,algorithm:4,align:4,all:4,alloc:4,allow:4,along:4,alpha:4,alreadi:4,also:[4,18],alter:4,altern:4,alwai:4,amount:4,an:[4,20],angl:9,angleaxi:[8,10,12,13],angular:[18,20],angular_dist:13,angularjacobian:18,ani:4,anoth:4,anotherth:4,aof:4,apart:4,api:[2,4],appear:4,append:4,appli:4,appropri:4,approxim:4,ar:4,area:4,aren:4,arg0:[8,9,10,11,12,13,14],arg1:[13,14],arg2:14,arg:[4,8,9,10,11,12,13,14,15,17,20],argument:4,around:17,arrai:4,art:4,asc:4,ascend:4,ask:4,assign:4,assist:4,associ:4,assum:4,assumpt:4,async:4,asynchron:4,atom:4,attempt:4,attribut:[4,5,8,9,10,12,13,14],auth:4,authent:4,author:4,auto:4,avail:4,avoid:17,await:4,axi:[4,9],back:4,base:[4,5,8,9,10,11,12,13,14,19],batch:4,bcast:4,becaus:4,been:4,befor:4,begin:4,behav:[4,20],behavior:4,being:4,belong:4,best:4,between:[4,16,18],beyond:4,bf:4,bgrewriteaof:4,bgsave:4,bind:4,bit:4,bitcount:4,bitfield:4,bitfield_ro:4,bitfieldoper:4,bitop:4,bitpo:4,bitwis:4,blmove:4,blmpop:4,block:4,blocking_timeout:4,bloom:4,blpop:4,bodi:17,bool:[4,8,9,10,12,13,14,19,21],border:4,both:4,box:4,broadcast:4,brpop:4,brpoplpush:4,bulk:4,bylex:4,bypass:4,byscor:4,bzmpop:4,bzpopmax:4,bzpopmin:4,c:[2,15,16,17,18],cach:4,call:4,callabl:4,callback:4,can:[4,17],cancel:4,cannot:4,cardin:4,cast:4,cat:4,categori:4,caus:[4,17],cc:18,center:4,certain:4,cf:4,ch:4,chang:4,channel:4,charact:4,check:4,circl:4,circular:4,claim:4,classmethod:4,clear:4,cli:4,client:4,client_getnam:4,client_getredir:4,client_id:4,client_info:4,client_kil:4,client_kill_filt:4,client_list:4,client_nam:4,client_no_evict:4,client_paus:4,client_repli:4,client_setnam:4,client_track:4,client_tracking_off:4,client_tracking_on:4,client_trackinginfo:4,client_unblock:4,client_unpaus:4,clientid:4,clip:20,close:4,closer:17,cluster:4,cm:4,code:4,collect:2,collis:4,column:20,combin:4,command:4,command_doc:4,command_getkeysandflag:4,command_list:4,command_nam:4,common:4,complet:4,complex:4,comput:[4,15,16,17,18],conceptu:4,concret:4,config:4,config_get:4,config_resetstat:4,config_rewrit:4,config_set:4,configur:4,conflict:4,conftest:4,conjug:13,connect:4,connectionpool:4,consid:4,consist:4,construct:4,constructor:4,consum:4,consumernam:4,contain:4,content:4,contenxt:4,context:4,continu:4,control:[2,17,18,20],conveni:4,coordin:4,copi:4,correspond:[4,20],could:4,count:4,counter:4,creat:4,createconsum:4,criteria:4,ctrl:0,ctrl_util:[0,15,16,17,18],ctrlutils_eigen:[8,9,10,11,12,13,14],current:[4,16,17,18,20],cursor:4,custom:4,cv:4,d:20,dafualt:4,damp:20,data:4,databas:4,datetim:4,db:4,dbsize:4,ddx:20,ddx_max:20,deal:4,debug:4,debug_object:4,decod:4,decr:4,decrbi:4,decrement:4,default_overflow:4,defin:4,delai:4,delconsum:4,delet:4,deliv:4,deliveri:4,delus:4,depend:4,deploy:4,deprec:4,desc:4,descend:4,describ:4,desir:[15,16,17,18,20],dest:4,destin:4,destination_db:4,destroi:4,detail:4,determin:4,dict:4,dictionari:4,did:4,differ:4,dimens:20,direct:4,dirti:4,disabl:4,disconnect:4,discov:4,disk:4,distanc:[4,20],distinct:4,doctor:4,doe:4,doesn:4,domain:4,don:4,dot:[13,15,17],doth:4,doubl:[8,10,12,13,15,17,18,20],dq:18,dryrun:4,dst:4,due:20,dump:4,dure:4,dw:[18,20],dw_max:20,dx:20,dynam:4,e:4,each:[4,17],earliest:4,echo:4,effect:17,eigen:[4,15,17,18,20],either:4,elaps:4,element:[4,17],els:4,empti:4,enabl:4,encod:4,end:4,engin:4,enough:4,entir:4,entri:4,entries_read:4,epoch:4,equal:[4,20],equival:[4,18],error:[4,16,18,20],eval:4,eval_ro:4,evalsha:4,evalsha_ro:4,even:4,event:4,everi:4,evict:4,ex:4,exactli:4,exampl:[4,17],exat:4,exce:4,except:4,exclus:4,execut:4,execute_command:4,exist:4,exit:4,expect:4,expir:4,expireat:4,expiretim:4,expiri:4,explicitli:4,expos:4,extend:4,extern:4,f:4,f_contigu:[8,9,10,11,12,13,15,17,20],fact:4,fail:4,failov:4,failur:4,fairest:4,fals:4,farquaternion:15,farther:[15,17],fcal_ro:4,fcall:4,fcall_ro:4,featur:4,feet:4,fetch:4,field:4,file:4,filter:4,find:4,finish:4,first:[4,20],first_list:4,flag:[4,8,9,10,11,12,13,15,17,20],float64:[8,9,10,11,12,13,14,15,16,17,18,20,21],flush:4,flushal:4,flushdb:4,fly:4,follow:[4,18],foo:4,foomod:4,foomodul:4,forc:4,forev:4,form:4,forth:4,found:4,frequenc:[4,5],from:[4,15,18],from_two_vector:13,from_url:4,ft:4,full:4,fulli:4,func:4,funcion:4,funcnam:4,function_delet:4,function_dump:4,function_flush:4,function_kil:4,function_list:4,function_load:4,function_restor:4,function_stat:4,furthest:4,futur:4,g:4,gain:20,gener:[4,20],genpass:4,geo:4,geoadd:4,geodist:4,geohash:4,geopo:4,georadiu:4,georadiusbymemb:4,geosearch:4,geosearchstor:4,geospati:4,get:[2,4],get_connection_kwarg:4,get_encod:4,get_imag:4,get_matrix:4,getbit:4,getdel:4,getex:4,getkeysandflag:4,getnam:4,getrang:4,getredir:4,getset:4,getus:4,github:1,given:[4,15,17,18,20],glob:4,go:18,goal:[17,20],got:4,grant:4,graph:4,greater:4,group:4,groupnam:4,gt:4,ha:[4,15,17],had:4,handi:4,handl:4,hash:4,hashed_password:4,have:4,hdel:4,head:4,height:4,hello:4,help:4,hexist:4,hget:4,hgetal:4,hide:4,highest:4,hincrbi:4,hincrbyfloat:4,hint:4,histori:4,hkei:4,hlen:4,hmget:4,hmset:4,hold:4,host:4,hostnam:4,how:4,howev:4,hrandfield:4,hscan:4,hscan_it:4,hset:4,hsetnx:4,hstrlen:4,http:4,hval:4,hyperloglog:4,i:4,iana:4,id:4,ident:[4,8,9,10,12,13,14],identifi:4,idl:4,idletim:4,idx:4,ignor:[4,16,20],illustr:4,imag:4,immedi:4,implement:4,includ:4,inclus:4,incr:4,incrbi:4,incrbyfloat:4,increment:4,indefinit:4,index:[2,4],index_nam:4,indic:4,influenc:4,info:4,inform:4,infotyp:4,initi:4,inject:4,input:4,insert:4,insid:4,instal:2,instanc:4,instead:4,instruct:1,integ:4,intention:4,intern:4,interpol:18,interpret:4,interrupt:4,intersect:4,interv:4,invalid:4,invers:[8,9,10,12,13,14],inversedynam:18,invok:4,io:4,ip:4,is_approx:[8,9,10,12,13,14],isn:4,item:4,iter:4,its:4,itself:4,j_w:18,januari:4,json:4,jsondecod:4,jsonencod:4,just:4,justid:4,keepttl:4,kei:4,kept:4,key1:4,key2:4,keys_and_arg:4,keyword:4,kill:4,kilomet:4,km:4,kp:20,kp_kv:20,kp_ori:18,kv:20,kv_ori:18,kwarg:[4,8,9,10,11,12,13,14,15,17,20],laddr:4,lag:4,larg:20,larger:4,largest:4,last:4,lastsav:4,lat:4,latenc:4,latency_doctor:4,latency_graph:4,latency_histogram:4,latency_histori:4,latency_latest:4,latency_reset:4,later:4,latest:4,latitud:4,law:[18,20],lc:4,least:4,left:4,len:4,length:4,less:[4,20],lexicograph:4,librari:4,life:4,like:[4,18,20],limit:4,lindex:4,linsert:4,list:4,listen:4,liter:4,live:4,ll:4,llen:4,lmove:4,lmpop:4,load:4,load_external_modul:4,loadex:4,local:4,localhost:4,locat:4,lock:4,lock_class:4,log:4,logic:4,lolwut:4,lon:4,longest:4,longitud:4,look:[4,18],lookat:16,lookaterror:16,loop:4,lost:4,lowest:4,lpo:4,lpop:4,lpush:4,lpushx:4,lrang:4,lrem:4,lset:4,lt:4,ltrim:4,lua:4,m:[4,11,20,21],mai:[4,17],major:4,make:4,make_affin:[8,10,12],malloc:4,manag:4,mani:4,map:4,mass:20,master:4,mat:4,match:4,matrix3d:18,matrix:[4,9,13,17,20],max:4,maximum:[4,20],maxlen:4,mean:4,mechan:4,member:4,memori:4,memory_malloc_stat:4,memory_purg:4,memory_stat:4,memory_usag:4,memoryview:4,mention:4,merg:4,messag:4,message_id:4,meta:4,metadata:4,meter:4,method:[4,5,8,9,10,11,12,13,14,19],mget:4,mi:4,microsecond:4,might:18,migrat:4,mile:4,millisecond:4,mimic:4,min:4,min_idle_tim:4,minid:4,minim:4,minimum:4,minmatchlen:4,miss:4,mkstream:4,mode:4,modifi:4,modul:[2,4],module_list:4,module_load:4,module_loadex:4,module_unload:4,moduleerror:4,monitor:4,more:4,most:4,move:4,ms:4,mset:4,msetnx:4,multipl:4,must:4,mutual:4,my:4,n:[4,11,20,21],name:4,namespac:4,ndarrai:[4,6,8,9,10,11,12,13,14,15,16,17,18,20,21],nearest:4,nearquaternion:17,necessari:4,need:4,neg:[4,15],negat:17,nest:4,new_master_nam:4,newer:4,next:4,nil:4,noack:4,node:4,noloop:4,nomkstream:4,non:4,none:[4,8,9,10,12,13,14,19],nopass:4,norm:13,normal:[4,13],nosav:4,notat:4,note:4,noth:4,notif:4,notimplementederror:4,now:4,num:4,num_kei:4,num_replica:4,number:4,numer:4,numkei:4,numpat:4,numpi:[8,9,10,11,12,13,14,15,16,17,18,20,21],numsub:4,nx:4,object:[4,5],observ:4,obtain:4,occurr:4,off:4,offset:4,ok:4,old:4,older:4,onc:4,one:[4,17,18],ones:4,ongo:4,onli:4,onto:4,oper:4,opspac:18,optin:4,option:[4,20],optout:4,order:4,org:4,ori:[15,17],orient:[15,16,17,18,20],orientation_error:16,orientationerror:18,origin:4,other:[4,17],otherwis:4,our:4,output:20,overhead:4,overload:[8,9,10,12,13,14,15,17,20],overridden:4,overwrit:4,overwritten:4,own:4,ownership:4,packag:4,pad:4,page:[1,2,4],pair:4,param:4,paramet:[4,16,18],pars:4,parse_respons:4,particular:4,partwai:4,pass:4,password:4,path:4,pattern:4,paus:4,payload:4,pd:20,pecifi:4,pel:4,pend:4,per:[4,20],percent:4,perform:4,permiss:4,persist:4,pexpir:4,pexpireat:4,pexpiretim:4,pfadd:4,pfcount:4,pfmerg:4,piec:4,ping:4,pipelin:4,place1:4,place2:4,place:4,plain:4,pleas:4,plu:4,point:4,polici:4,pool:4,pop:4,popul:4,port:4,portion:4,posit:[4,17,20],possibl:4,practic:4,prec:[8,9,10,12,13,14],prefer:4,prefix:4,prerot:[8,10,12],present:4,prevent:4,previou:4,previous:4,prior:4,privileg:4,process:4,produc:[16,18],product:[15,17],progress:4,promot:4,properli:4,prov:4,provid:4,psetex:4,psync:4,pttl:4,publish:4,pubsub:4,pubsub_channel:4,pubsub_numpat:4,pubsub_numsub:4,pure:4,purg:4,push:4,px:4,pxat:4,py:4,pybind11_builtin:[8,9,10,11,12,14,19],pybind11_object:[8,9,10,11,12,14,19],python:[2,4],quarternion:17,quat:[15,16,17,18,20],quat_d:[16,18,20],quat_ref:[15,17],quat_refer:[15,17],quaternion:[8,10,12,15,16,17,18,20],quaterniond:9,queri:4,querystr:4,queue:4,quit:4,quorum:4,r:4,radiu:4,rais:4,random:4,randomkei:4,randomli:4,rang:4,rank:4,rather:4,raw:4,reach:[4,20],reachabl:4,read:4,readonli:4,readwrit:4,recent:4,reclam:4,record:4,rectangl:4,rectangular:4,redi:4,redirect:4,redisjson:4,rediss:4,reduc:4,refcount:4,refer:[15,17],reflect:4,refvalu:4,regardless:4,regist:4,register_script:4,regular:4,reimport:4,rel:4,releas:4,remain:4,rememb:4,remov:4,renam:4,renamenx:4,replac:4,repli:4,replic:4,replica:4,replicaof:4,replicationid:4,report:4,repres:[4,15,17,20],request:4,requir:18,reset:4,reset_channel:4,reset_kei:4,reset_password:4,resetstat:4,respect:[4,20],respons:4,responseerror:4,restart:4,restor:4,restrict:4,result:4,retain:4,retri:4,retriev:4,retry_on_error:4,retry_on_timeout:4,retrycount:4,revers:4,rewrit:4,right:4,robot:2,role:4,rotat:[8,10,12,16,17],round:4,row:20,rpop:4,rpoplpush:4,rpush:4,rpushx:4,rtype:4,rule:4,run:4,runtim:4,s:[4,6],sadd:4,safe:4,same:[4,15,17],sampl:4,save:4,scan:4,scan_it:4,scard:4,schedul:4,scheme:4,score:4,score_cast_func:4,script:4,script_exist:4,script_flush:4,script_kil:4,script_load:4,sdiff:4,sdiffstor:4,search:[2,4],second:[4,18,20],second_list:4,section:4,see:[1,4],seen:4,select:4,selector:4,self:[8,9,10,11,12,13,14,19],semant:4,send:4,sentinel:4,sentinel_ckquorum:4,sentinel_failov:4,sentinel_flushconfig:4,sentinel_get_master_addr_by_nam:4,sentinel_mast:4,sentinel_monitor:4,sentinel_remov:4,sentinel_reset:4,sentinel_sentinel:4,sentinel_set:4,sentinel_slav:4,sequenc:4,seri:4,serial:4,server:4,service_nam:4,set:[4,8,9,10,12,13,14],set_from_two_vector:13,set_ident:[8,10,12,13],set_imag:4,set_matrix:4,set_response_callback:4,setbit:4,setex:4,setid:4,setnam:4,setnx:4,setrang:4,setus:4,sever:4,sha:4,shape:4,shard_hint:4,shebang:4,shortest:[16,17],should:4,shutdown:4,side:4,similar:4,simpl:4,simul:4,sinc:4,singl:4,single_connection_cli:4,singular:17,sinter:4,sintercard:4,sinterstor:4,sismemb:4,size:4,skip:4,skipm:4,slave:4,slaveof:4,sleep:4,slerp:[13,18],slice:4,slightli:4,slowlog:4,slowlog_get:4,slowlog_len:4,slowlog_reset:4,smallest:4,smember:4,smismemb:4,smove:4,so:4,sock:4,socket:4,sole:4,solv:11,some:4,someth:4,sometim:4,soon:4,sort:4,sort_ro:4,sourc:4,special:[4,20],specif:4,specifi:4,specific_argu:4,spend:4,spike:4,spop:4,spring:20,squared_norm:13,srandmemb:4,src:4,srem:4,sscan:4,sscan_it:4,ssl:4,start:[2,4],start_id:4,stat:4,state:4,statement:4,statist:4,statu:4,stderr:19,stdout:19,step:4,stock:4,storag:4,store:4,store_dist:4,storedist:4,str:4,straightforward:4,stralgo:4,stream:4,string:4,strlen:4,structur:4,style:4,subcommand:4,subscrib:4,subscript:4,subsequ:4,subset:4,substr:4,success:4,successfulli:4,sum:4,summari:4,sunion:4,sunionstor:4,suppli:4,support:4,suspend:4,svd_epsilon:21,swap:4,swapdb:4,sync:4,sync_typ:4,synchron:4,system:[4,20],t:4,tail:4,take:4,tau:18,tcp:4,tdigest:4,tell:4,test:4,test_client_repli:4,test_connect:4,test_loading_external_modul:4,text:4,than:[4,20],thei:4,them:4,thi:[4,16,17,18,20],those:4,thread:4,thread_loc:4,three:4,through:4,time:4,time_m:4,timedelta:4,timelin:4,timeout:4,timeouterror:4,timeseri:4,timestamp:4,token:4,top:4,topk:4,total:4,touch:4,track:4,trackinginfo:4,transact:4,transfer:4,translat:[8,10,12],triad:4,trim:4,truncat:4,ts:4,ttl:4,tupl:[4,20,21],turn:4,two:[4,16,17,18],type:[4,6],unblock:4,union:4,uniqu:4,unit:4,unit_random:13,unix:4,unless:4,unlik:[4,16],unlimit:4,unlink:4,unload:4,unpaus:4,unquot:4,unspecifi:4,until:4,unwatch:4,up:4,updat:4,upgrad:4,upper:4,uri:4,url:4,urllib:4,us:[4,17],usag:4,user:4,usernam:4,util:0,val:4,valid:4,valu:[4,20],value1:4,value2:4,valueerror:4,vari:4,variant:4,varieti:4,vec:16,vec_d:16,vector3d:18,vector:[4,16,20],vectorxd:18,veloc:[18,20],version:4,version_numb:4,via:4,w:[13,20],w_err:[18,20],wa:4,wai:[4,17],wait:4,want:4,watch:4,we:4,weight:4,well:[4,20],were:4,what:4,when:[4,20],where:[4,17,20],whether:4,which:[4,17],whichev:4,whoami:4,whom:4,whose:4,width:4,wildcard:4,win:4,withcod:4,withcoord:4,withdist:4,withhash:4,within:4,withmatchlen:4,without:4,withscor:4,withvalu:4,won:4,word:4,work:4,worker:4,would:4,wrap:4,write:4,writeabl:13,www:4,x:[13,20],x_de:20,x_err:20,xack:4,xadd:4,xautoclaim:4,xclaim:4,xdel:4,xgroup:4,xgroup_creat:4,xgroup_createconsum:4,xgroup_delconsum:4,xgroup_destroi:4,xgroup_setid:4,xinfo:4,xinfo_consum:4,xinfo_group:4,xinfo_stream:4,xlen:4,xpend:4,xpending_rang:4,xrang:4,xread:4,xreadgroup:4,xrevrang:4,xtrim:4,xx:4,xyz:4,y:[13,20],ye:4,yet:4,yield:4,you:4,your:4,z:[13,20],zadd:4,zcard:4,zcount:4,zdiff:4,zdiffstor:4,zincrbi:4,zinter:4,zintercard:4,zinterstor:4,zlexcount:4,zmpop:4,zmscore:4,zpopmax:4,zpopmin:4,zrandmemb:4,zrang:4,zrangebylex:4,zrangebyscor:4,zrangestor:4,zrank:4,zrem:4,zremrangebylex:4,zremrangebyrank:4,zremrangebyscor:4,zrevrang:4,zrevrangebylex:4,zrevrangebyscor:4,zrevrank:4,zscan:4,zscan_it:4,zscore:4,zset:4,zunion:4,zunionstor:4},titles:["API Documentation","Getting Started","ctrl-utils","ctrlutils","ctrlutils.RedisClient","ctrlutils.Timer","ctrlutils.decode_matlab","ctrlutils.eigen","ctrlutils.eigen.Affine3d","ctrlutils.eigen.AngleAxisd","ctrlutils.eigen.Isometry3d","ctrlutils.eigen.LdltMatrixXd","ctrlutils.eigen.Projective3d","ctrlutils.eigen.Quaterniond","ctrlutils.eigen.Translation3d","ctrlutils.far_quaternion","ctrlutils.lookat_error","ctrlutils.near_quaternion","ctrlutils.orientation_error","ctrlutils.ostream_redirect","ctrlutils.pd_control","ctrlutils.pseudo_inverse"],titleterms:{affine3d:8,angleaxisd:9,api:0,c:0,ctrl:2,ctrlutil:[3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21],decode_matlab:6,document:[0,2],eigen:[7,8,9,10,11,12,13,14],far_quaternion:15,get:1,indic:2,instal:1,isometry3d:10,ldltmatrixxd:11,lookat_error:16,near_quaternion:17,orientation_error:18,ostream_redirect:19,pd_control:20,projective3d:12,pseudo_invers:21,python:0,quaterniond:13,rediscli:4,start:1,tabl:2,timer:5,translation3d:14,util:2}})