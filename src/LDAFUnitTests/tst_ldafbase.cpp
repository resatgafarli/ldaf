#include "tst_ldafbase.h"

using ::testing::AtLeast;

/*--------------------------  LDAFCallBackObject -----------------------------------*/
TEST(LDAFCallBackObjectTest, LDAFCallBackObjectConstruction){
  MockQObject * object = new MockQObject();
  LDAFCallBackObject callBackObject(object,"TestFunction");

  EXPECT_EQ(callBackObject.getObjectPointer(),object);
  EXPECT_EQ(callBackObject.getFunctionName(),"TestFunction");
}

TEST(LDAFCallBackObjectTest, LDAFCallBackObjectCopy){
  MockQObject * object = new MockQObject();
  LDAFCallBackObject callBackObject(object,"TestFunction");
  LDAFCallBackObject copyCallBackObject = callBackObject; 

  EXPECT_EQ(copyCallBackObject.getObjectPointer(),object);
  EXPECT_EQ(copyCallBackObject.getFunctionName(),"TestFunction");
}


/*----------------------------- LDAFMessage Types --------------------------------- */
TEST(LDAFMessageTypeTest, LDAFMessageTypeConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    LDAFCallBackObject callBackObject(&mockQObject,"testFunction");
    MockLDAFMessageType ldafMessage(&mockLDAFbase,callBackObject);
    EXPECT_EQ(ldafMessage.getBasicObject(),&mockLDAFbase);
    EXPECT_EQ(ldafMessage.getCallBackObject().getObjectPointer() ,&mockQObject);
    EXPECT_EQ(ldafMessage.getCallBackObject().getFunctionName(),"testFunction");    
}

TEST(LDAFUrlTypeTest, LDAFUrlTypeConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    QUrl url;
    url.setPath("/Example/Resource/Path");
    LDAFCallBackObject callBackObject(&mockQObject,"testFunction");
    MockLDAFUrl ldafUrl(url,&mockLDAFbase,callBackObject);
    EXPECT_EQ(ldafUrl.getBasicObject(),&mockLDAFbase);
    EXPECT_EQ(ldafUrl.getCallBackObject().getObjectPointer(),&mockQObject);
    EXPECT_EQ(ldafUrl.getCallBackObject().getFunctionName(),"testFunction");  
    EXPECT_EQ(ldafUrl.getUrl().path(),"/Example/Resource/Path");  
}

TEST(LDAFJsonTypeTest, LDAFJsonTypeConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    QJsonObject jsonObject;

    QString jscript = "{\"par1\":\"val1\",\"par2\":\"val2\"}";
    QJsonDocument jdoc = QJsonDocument::fromJson(jscript.toUtf8());
    LDAFCallBackObject callBackObject(&mockQObject,"testFunction");
    MockLDAFJson ldafJson(jdoc.object(),&mockLDAFbase,callBackObject);
    EXPECT_EQ(ldafJson.getBasicObject(),&mockLDAFbase);
    EXPECT_EQ(ldafJson.getCallBackObject().getObjectPointer(),&mockQObject);
    EXPECT_EQ(ldafJson.getCallBackObject().getFunctionName(),"testFunction");
    EXPECT_EQ(ldafJson.getJsonObject()["par1"],"val1");  
    EXPECT_EQ(ldafJson.getJsonObject()["par2"],"val2");  
}
/*-------------------------- LDAF Command Pattern  Tests --------------------------*/

TEST(LDAFCommandTest,LDAFCommandConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    LDAFCallBackObject callBackObject(&mockQObject,QString());
    MockLDAFMessageType * messageType = new MockLDAFMessageType(&mockLDAFbase,callBackObject);
    
    MockLDAFCommand ldafCommand(messageType,&LDAFMessageType::setMessage);
    EXPECT_EQ(ldafCommand.getMessageObject(),messageType);
}

TEST(LDAFCommandTest,LDAFCommandExection){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    LDAFCallBackObject callBackObject(&mockQObject,QString());
    MockLDAFMessageType * messageType = new MockLDAFMessageType(&mockLDAFbase,callBackObject);
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
    LDAFCallBackObject callBackObject(&mockQObject,QString());
    MockLDAFUrl * urlMessage = new MockLDAFUrl(QUrl(),&mockLDAFbase,callBackObject);
    LDAFCommand ldafCommand(urlMessage,&LDAFMessageType::setMessage);
    
    EXPECT_CALL(*urlMessage,setMessage()).Times(AtLeast(1));
    ldafCommand.executeCommand();
}

TEST(LDAFCommandTest,LDAFCommandJsonExecution){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    LDAFCallBackObject callBackObject(&mockQObject,QString());
    MockLDAFJson * jsonMessage = new MockLDAFJson(QJsonObject(),&mockLDAFbase,callBackObject);
    LDAFCommand ldafCommand(jsonMessage,&LDAFMessageType::setMessage);
    
    EXPECT_CALL(*jsonMessage,setMessage()).Times(AtLeast(1));
    ldafCommand.executeCommand();
}

/*-------------------------- LDAFCommandListProcessor ----------------------------*/


/*
TEST(LDAFBaseTest,LDAFBase_setURLMessage){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    QUrl url;
    url.setPath("/Example/Resource/Path");
    LDAFUrl * urlMessage = new LDAFUrl(url,&mockLDAFbase,&mockQObject,QString());
    LDAFCommand ldafCommand(urlMessage,&LDAFMessageType::setMessage);
    
    EXPECT_CALL(mockLDAFbase,setURLMessage()).Times(AtLeast(1));
    ldafCommand.executeCommand();


}
*/