var catcher=require('./catcher/Catcher');
var catchExportRules=catcher.catchExportRules;
var catchOutline=catcher.catchOutline;
var queryAllSource=catcher.queryAllSource;

var assetsExportor=require('./AssetsExportor');
module.exports = {
    //用于获取Canvas上所有export rule的信息
    'getExportRules':function(event){
        var canvas = cc.find('Canvas');
        if(canvas){
            var exportRules=[];
            var temp=catchExportRules(canvas);
            for(var i=0;i<temp.length;i++){
                var obj={};
                obj.ruleName=temp[i].ruleName;
                obj.src=temp[i].src_Node.name;
                obj.dst=temp[i].dstPath;
                exportRules.push(obj);
            }
            var data=JSON.stringify(exportRules);
            if(event.reply) {
                event.reply(data);
            }
        }
    },
    //用于获取所有选中的export rule指定的node的数据
    'getNode': function (event,exportRuleNames,projectPath) {
        var canvas = cc.find('Canvas');
        var exportRules=catchExportRules(canvas);
        
        if('export-all-rule'===exportRuleNames){
            exportRuleNames=[];
            exportRules.forEach(function(exportRule){
                exportRuleNames.push(exportRule.ruleName);
            });
        }else
            exportRuleNames=JSON.parse(exportRuleNames);
        
        function hasName(name){
            for(var i=0;i<exportRuleNames.length;i++){
                if(exportRuleNames[i]===name)
                    return true;
            }
            return false;
        }

        var nodes=[];
        for(var i=0;i<exportRules.length;i++){
            var exportRule=exportRules[i];
            if(hasName(exportRule.ruleName)){
                if(exportRule.resFolder&&exportRule.resFolder!='')
                    assetsExportor.setFolder(exportRule.resFolder,projectPath);
                    var obj=new Object();
                    obj.nodeData=catchOutline(exportRule);
                    obj.dstPath=exportRule.dstPath;
                    obj.language=exportRule.language;
                    obj.export_independent_file=exportRule.export_independent_file;
                    obj.namespace=exportRule.namespace;
                    nodes.push(obj);
                    if(exportRule.resFolder&&exportRule.resFolder!='')
                        assetsExportor.startCopy();
                }
        }
        if(event.reply) {
            event.reply(JSON.stringify(nodes));
        }
    },
    //查找资源
    'querySource':function(event){
        var canvas = cc.find('Canvas');
        queryAllSource(canvas,event);
    }
};