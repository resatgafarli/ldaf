#include "tst_ldafbase.h"

using ::testing::AtLeast;
TEST(LDAFMessageTypeTest, LDAFMessageTypeConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    MockLDAFMessageType ldafMessage(&mockLDAFbase,&mockQObject,"testFunction");
    EXPECT_EQ(ldafMessage.getBasicObject(),&mockLDAFbase);
    EXPECT_EQ(ldafMessage.getCallBackObject(),&mockQObject);
    EXPECT_EQ(ldafMessage.getCallBackJSFunc(),"testFunction");    
}

TEST(LDAFUrlTypeTest, LDAFUrlTypeConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    QUrl url;
    url.setPath("/Example/Resource/Path");
    MockLDAFUrl ldafUrl(url,&mockLDAFbase,&mockQObject,"testFunction");
    EXPECT_EQ(ldafUrl.getBasicObject(),&mockLDAFbase);
    EXPECT_EQ(ldafUrl.getCallBackObject(),&mockQObject);
    EXPECT_EQ(ldafUrl.getCallBackJSFunc(),"testFunction");  
    EXPECT_EQ(ldafUrl.getUrl().path(),"/Example/Resource/Path");  
}

TEST(LDAFJsonTypeTest, LDAFJsonTypeConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    QJsonObject jsonObject;

    QString jscript = "{\"par1\":\"val1\",\"par2\":\"val2\"}";
    QJsonDocument jdoc = QJsonDocument::fromJson(jscript.toUtf8());

    MockLDAFJson ldafJson(jdoc.object(),&mockLDAFbase,&mockQObject,"testFunction");
    EXPECT_EQ(ldafJson.getBasicObject(),&mockLDAFbase);
    EXPECT_EQ(ldafJson.getCallBackObject(),&mockQObject);
    EXPECT_EQ(ldafJson.getCallBackJSFunc(),"testFunction");
    EXPECT_EQ(ldafJson.getJsonObject()["par1"],"val1");  
    EXPECT_EQ(ldafJson.getJsonObject()["par2"],"val2");  
}

TEST(LDAFCommandTest,LDAFCommandConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    MockLDAFMessageType * messageType = new MockLDAFMessageType(&mockLDAFbase,&mockQObject,"testFunction");
    
    MockLDAFCommand ldafCommand(messageType,&LDAFMessageType::setMessage);
    EXPECT_EQ(ldafCommand.getMessageObject(),messageType);
}

TEST(LDAFCommandTest,LDAFCommandExection){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    MockLDAFMessageType * messageType = new MockLDAFMessageType(&mockLDAFbase,&mockQObject,"testFunction");
    EXPECT_CALL(*messageType,setMessage()).Times(AtLeast(3));

    MockLDAFCommand ldafCommand(messageType,&LDAFMessageType::setMessage);
    ldafCommand.executeCommand();//1
    ldafCommand.executeCommand();//2
    ldafCommand.executeCommand();//2
    
    EXPECT_EQ(ldafCommand.getMessageObject(),messageType);
    EXPECT_EQ(ldafCommand.getFunctionPointer(),&LDAFMessageType::setMessage);     
}

TEST(LDAFCommandTest,LDAFCommandUrlExecution){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    MockLDAFUrl * urlMessage = new MockLDAFUrl(QUrl(),&mockLDAFbase,&mockQObject,"testFunction");
    LDAFCommand ldafCommand(urlMessage,&LDAFMessageType::setMessage);
    
    EXPECT_CALL(*urlMessage,setMessage()).Times(AtLeast(1));
    ldafCommand.executeCommand();
}

TEST(LDAFCommandTest,LDAFCommandJsonExecution){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    MockLDAFJson * jsonMessage = new MockLDAFJson(QJsonObject(),&mockLDAFbase,&mockQObject,"testFunction");
    LDAFCommand ldafCommand(jsonMessage,&LDAFMessageType::setMessage);
    
    EXPECT_CALL(*jsonMessage,setMessage()).Times(AtLeast(1));
    ldafCommand.executeCommand();
}