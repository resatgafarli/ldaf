#include "tst_ldafbase.h"

using ::testing::AtLeast;
using ::testing::_;
using ::testing::Invoke;
using ::testing::InSequence;

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
TEST_F(LDAFCommandListProcessorTest,LDAFCommandListProcessorAddUrlCommand){
    fillUrlQueue();
    ASSERT_EQ(commandListProcessor.getCommandlist().size(),urlList.size());
    
    int i = 0;
    for (auto command : commandListProcessor.getCommandlist()){
        const LDAFUrl * const ldafMess = static_cast<const LDAFUrl * const>(command->getMessageObject());
        ASSERT_EQ(ldafMess->getUrl(), urlList.at(i));
        i++;
    }
}
TEST_F(LDAFCommandListProcessorTest,LDAFCommandListProcessorAddJSONCommand){
    fillJsonQueue();    
    ASSERT_EQ(commandListProcessor.getCommandlist().size(),jsonList.size());

    int i = 0;
    for (auto command : commandListProcessor.getCommandlist()){
        const LDAFJson * const ldafMess = static_cast<const LDAFJson * const>(command->getMessageObject());
        ASSERT_EQ(ldafMess->getJsonObject(), jsonList.at(i));
        i++;
    }
}

TEST_F(LDAFCommandListProcessorTest,LDAFCommandListProcessorTotalSize){
    fillUrlQueue();
    fillJsonQueue();

    int totalListSize = jsonList.size()+urlList.size();
    EXPECT_TRUE(commandListProcessor.getCommandlist().size() == totalListSize);
}


TEST_F(LDAFCommandListProcessorTest,LDAFCommandListProcessorHasNextPrev){
    EXPECT_TRUE(commandListProcessor.hasNext()==false);
    fillUrlQueue();

    EXPECT_TRUE(commandListProcessor.hasNext()==true);
    EXPECT_TRUE(commandListProcessor.hasPrev()==false);

    commandListProcessor.processAllForward();
    EXPECT_TRUE(commandListProcessor.hasNext()==false);
    EXPECT_TRUE(commandListProcessor.hasPrev()==true);

    commandListProcessor.processAllBackward();
    EXPECT_TRUE(commandListProcessor.hasNext()==true);
    EXPECT_TRUE(commandListProcessor.hasPrev()==false);

}


TEST_F(LDAFCommandListProcessorTest,LDAFCommandListProcessorFullForwardValidation){
   InSequence s;
   fillUrlQueue();
   fillJsonQueue();
   QListIterator<QUrl> urlIt = urlList;
   urlIt.toFront();
   while(urlIt.hasNext()){
       QUrl url = urlIt.next(); 
        EXPECT_CALL(mockLDAFBase,setURLMessage(url,_)).Times(1);
   }
   
   QListIterator<QJsonObject> jsonIt = jsonList;
   jsonIt.toFront();
   while(jsonIt.hasNext()){
       QJsonObject json = jsonIt.next(); 
        EXPECT_CALL(mockLDAFBase,setJsonMessage(json,_)).Times(1);
   }

   commandListProcessor.processAllForward();
}


TEST_F(LDAFCommandListProcessorTest,LDAFCommandListProcessorFullForwardBackwardTwiceValidation){
   fillUrlQueue();
   fillJsonQueue();
   
   for (int i=0;i<urlList.size();++i){
       EXPECT_CALL(mockLDAFBase,setURLMessage(urlList.at(i),_)).Times(2);
   }
   for (int i=0;i<jsonList.size()-1;++i){
       EXPECT_CALL(mockLDAFBase,setJsonMessage(jsonList.at(i),_)).Times(2); 
   }     
   //The last command is also current command at the end of processAllForward,
   //In this case when processAllBackward is called last command will not be called twice.
    if (jsonList.size()!=0)  
        EXPECT_CALL(mockLDAFBase,setJsonMessage(jsonList.at(jsonList.size()-1),_)).Times(1); 

   commandListProcessor.processAllForward();
   commandListProcessor.processAllBackward();
}

TEST_F(LDAFCommandListProcessorTest,LDAFCommandListProcessorFullForwardBackwardTripleValidation){
   fillUrlQueue();
   fillJsonQueue();

   //In this case first command will not be triple called.
    if (urlList.size()!=0)            
        EXPECT_CALL(mockLDAFBase,setURLMessage(urlList.at(0),_)).Times(2); 

   for (int i=1;i<urlList.size();++i){
       EXPECT_CALL(mockLDAFBase,setURLMessage(urlList.at(i),_)).Times(3);
   }

   for (int i=0;i<jsonList.size()-1;++i){
       EXPECT_CALL(mockLDAFBase,setJsonMessage(jsonList.at(i),_)).Times(3); 
   }     
   //The last command is also current command at the end of processAllForward,
   //In this case when processAllBackward is called last command will not be called twice.
   if (jsonList.size()!=0)               
        EXPECT_CALL(mockLDAFBase,setJsonMessage(jsonList.at(jsonList.size()-1),_)).Times(2); 

   commandListProcessor.processAllForward();
   commandListProcessor.processAllBackward();
   commandListProcessor.processAllForward();
}
