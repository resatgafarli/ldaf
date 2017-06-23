/******************************************************
Author: resat.gafarli@gmail.com
File: tst_ldafbase.cpp
Date: 6/7/2017
License: GPL-3.0
*******************************************************/
#include "tst_ldafbase.h"

using ::testing::AtLeast;
using ::testing::_;
using ::testing::Invoke;
using ::testing::InSequence;

/*--------------------------  LDAFCallBackObject -----------------------------------*/
TEST(LDAFCallBackObjectTest, LDAFCallBackObjectConstruction){
  MockQObject * object = new MockQObject();
  LDAFCallBackObject callBackObject(object,"");

  EXPECT_EQ(callBackObject.getObjectPointer(),object);
  EXPECT_EQ(callBackObject.getFunctionName(),"");
}

TEST(LDAFCallBackObjectTest, LDAFCallBackObjectCopy){
  MockQObject * object = new MockQObject();
  LDAFCallBackObject callBackObject(object,"");
  LDAFCallBackObject copyCallBackObject = callBackObject; 

  EXPECT_EQ(copyCallBackObject.getObjectPointer(),object);
  EXPECT_EQ(copyCallBackObject.getFunctionName(),"");
}


/*----------------------------- LDAFMessage Types --------------------------------- */
TEST(LDAFMessageTypeTest, LDAFMessageTypeConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    LDAFCallBackObject callBackObject(&mockQObject,"");
    MockLDAFMessageType ldafMessage(&mockLDAFbase,callBackObject);
    EXPECT_EQ(ldafMessage.getBasicObject(),&mockLDAFbase);
    EXPECT_EQ(ldafMessage.getCallBackObject().getObjectPointer() ,&mockQObject);
    EXPECT_EQ(ldafMessage.getCallBackObject().getFunctionName(),"");    
}

TEST(LDAFUrlTypeTest, LDAFUrlTypeConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    QUrl url;
    url.setPath("/Example/Resource/Path");
    LDAFCallBackObject callBackObject(&mockQObject,"");
    MockLDAFUrl ldafUrl(url,&mockLDAFbase,callBackObject);
    EXPECT_EQ(ldafUrl.getBasicObject(),&mockLDAFbase);
    EXPECT_EQ(ldafUrl.getCallBackObject().getObjectPointer(),&mockQObject);
    EXPECT_EQ(ldafUrl.getCallBackObject().getFunctionName(),"");  
    EXPECT_EQ(ldafUrl.getUrl().path(),"/Example/Resource/Path");  
}

TEST(LDAFJsonTypeTest, LDAFJsonTypeConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    QJsonObject jsonObject;

    QString jscript = "{\"par1\":\"val1\",\"par2\":\"val2\"}";
    QJsonDocument jdoc = QJsonDocument::fromJson(jscript.toUtf8());
    LDAFCallBackObject callBackObject(&mockQObject,"");
    MockLDAFJson ldafJson(jdoc.object(),&mockLDAFbase,callBackObject);
    EXPECT_EQ(ldafJson.getBasicObject(),&mockLDAFbase);
    EXPECT_EQ(ldafJson.getCallBackObject().getObjectPointer(),&mockQObject);
    EXPECT_EQ(ldafJson.getCallBackObject().getFunctionName(),"");
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

    EXPECT_CALL(mockLDAFBase,setURLMessage(_,_)).Times(AtLeast(1));

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

TEST_F(LDAFCommandListProcessorTest,LDAFCommandListProcessorReporcessCurrent){
   fillUrlQueue();
   
   int mid_command = urlList.size()/2;
   if (mid_command<=0) return; 

   EXPECT_CALL(mockLDAFBase,setURLMessage(_,_)).Times(mid_command);
   for(int i = 0; i < mid_command;++i){
       commandListProcessor.processForwardByOne();
   }

   if(mid_command>=1){
        EXPECT_CALL(mockLDAFBase,setURLMessage(urlList.at(mid_command-1),_)).Times(2);
        commandListProcessor.reProcessCurrent();
        commandListProcessor.reProcessCurrent();
   }
}

/*------------------------- LDAFBase ------------------------------------- */
TEST_F(LDAFBaseTest,LDAFBaseConfigurationValidation){
    EXPECT_TRUE(firstLDAFBase->getServerResourcePath() == "ServerResourcePath");
    EXPECT_TRUE(firstLDAFBase->getHomePagePath() == "HomePage");
    EXPECT_TRUE(secondLDAFBase->getServerResourcePath() == "ServerResourcePath");
    EXPECT_TRUE(secondLDAFBase->getHomePagePath() == "HomePage");
}

TEST_F(LDAFBaseTest,LDAFBaseReceiverObjectValidation){
    connectLDAFBases();
    EXPECT_TRUE(firstLDAFBase->getReceiverObject() == secondLDAFBase);
    EXPECT_TRUE(secondLDAFBase->getReceiverObject() == firstLDAFBase);
}

TEST_F(LDAFBaseTest,LDAFBaseMessageBroadcastCounter){
    connectLDAFBases();
    addUrlCommands();
    addJsonCommands();    

    EXPECT_CALL(*secondLDAFBase,setURLMessage(_,_)).Times(urlList.size()); 
    EXPECT_CALL(*secondLDAFBase,setJsonMessage(_,_)).Times(jsonList.size());

    firstLDAFBase->processAllForward();
}

TEST_F(LDAFBaseTest,LDAFBaseMessageForwardBroadcastValidation){
    connectLDAFBases();
    addUrlCommands();
    addJsonCommands();    
    for (auto url: urlList){
        EXPECT_CALL(*secondLDAFBase,setURLMessage(url,_)).Times(1); 
    }
    
    for (auto json: jsonList){
        EXPECT_CALL(*secondLDAFBase,setJsonMessage(json,_)).Times(1); 
    }

    firstLDAFBase->processAllForward();
}

TEST_F(LDAFBaseTest,LDAFBaseMessageBothSideBroadcastValidation){
    connectLDAFBases();
    addUrlCommands();
    addJsonCommands();    
   
   for (int i=0;i<urlList.size();++i){
       EXPECT_CALL(*secondLDAFBase,setURLMessage(urlList.at(i),_)).Times(2);
   }
   for (int i=0;i<jsonList.size()-1;++i){
       EXPECT_CALL(*secondLDAFBase,setJsonMessage(jsonList.at(i),_)).Times(2); 
   }     
   //The last command is also current command at the end of processAllForward,
   //In this case when processAllBackward is called last command will not be called twice.
    if (jsonList.size()!=0)  
        EXPECT_CALL(*secondLDAFBase,setJsonMessage(jsonList.at(jsonList.size()-1),_)).Times(1); 

    firstLDAFBase->processAllForward();
    firstLDAFBase->processAllBackward();
}

TEST_F(LDAFBaseTest,LDAFBaseMessageBothSideResponseValidation){
   connectLDAFBases();
   addUrlCommands();
   addJsonCommands();    
   
   for (int i=0;i<urlList.size();++i){
       ON_CALL(*secondLDAFBase,setURLMessage(urlList.at(i),_))
        .WillByDefault(Invoke(this,&LDAFBaseTest::setResponseUrlMessage));
       
       EXPECT_CALL(*secondLDAFBase,setURLMessage(urlList.at(i),_)).Times(2);
       EXPECT_CALL(*firstLDAFBase,setURLMessage(urlList.at(i),_)).Times(2);
   }

   for (int i=0;i<jsonList.size()-1;++i){
       ON_CALL(*secondLDAFBase,setJsonMessage(jsonList.at(i),_))
        .WillByDefault(Invoke(this,&LDAFBaseTest::setResponseJsonMessage));

       EXPECT_CALL(*secondLDAFBase,setJsonMessage(jsonList.at(i),_)).Times(2); 
       EXPECT_CALL(*firstLDAFBase,setJsonMessage(jsonList.at(i),_)).Times(2); 
   }     
   //The last command is also current command at the end of processAllForward,
   //In this case when processAllBackward is called last command will not be called twice.
    if (jsonList.size()!=0){
        ON_CALL(*secondLDAFBase,setJsonMessage(jsonList.at(jsonList.size()-1),_))
        .WillByDefault(Invoke(this,&LDAFBaseTest::setResponseJsonMessage));

        EXPECT_CALL(*secondLDAFBase,setJsonMessage(jsonList.at(jsonList.size()-1),_)).Times(1); 
        EXPECT_CALL(*firstLDAFBase,setJsonMessage(jsonList.at(jsonList.size()-1),_)).Times(1); 
    }  

    firstLDAFBase->processAllForward();
    firstLDAFBase->processAllBackward();
}

TEST_F(LDAFBaseTest,LDAFBaseMessageTwiceReprocessCurrentValidation){

   int mid_command = urlList.size()/2;
   if (mid_command<=0) return; 

   connectLDAFBases();
   addUrlCommands();

   EXPECT_CALL(*secondLDAFBase,setURLMessage(_,_)).Times(mid_command);
   for(int i = 0; i < mid_command;++i){
       firstLDAFBase->processForwardByOne();
   }

   if(mid_command>=1){
       ON_CALL(*secondLDAFBase,setURLMessage(urlList.at(mid_command-1),_))
        .WillByDefault(Invoke(this,&LDAFBaseTest::setResponseUrlMessage));
       
        EXPECT_CALL(*secondLDAFBase,setURLMessage(urlList.at(mid_command-1),_)).Times(3);
        EXPECT_CALL(*firstLDAFBase,setURLMessage(urlList.at(mid_command-1),_)).Times(3);
       
        firstLDAFBase->reProcessCurrent();//1
        firstLDAFBase->reProcessCurrent();//2
        firstLDAFBase->reProcessCurrent();//3
   }
   
}   

TEST_F(LDAFBaseTest,LDAFBaseCallBackObjectFunctionCallUrlValidation){
    connectLDAFBases();

    MockQObject mockQObject; 
    //testCallbackFunction is name of slot defined in MockQObject
    LDAFCallBackObject callBackObject = LDAFCallBackObject(&mockQObject,"testCallBackFunction");

    for (auto url: urlList){
        firstLDAFBase->addCommand(url,callBackObject);
    }
    
   
    for (auto url: urlList){
       ON_CALL(*secondLDAFBase,setURLMessage(url,callBackObject))
        .WillByDefault(Invoke(this,&LDAFBaseTest::setResponseUrlMessage));
        
        EXPECT_CALL(*secondLDAFBase,setURLMessage(url,callBackObject)).Times(1);
        EXPECT_CALL(*firstLDAFBase,setURLMessage(url,callBackObject)).Times(1); 

      ON_CALL(*firstLDAFBase,setURLMessage(url,callBackObject))
        .WillByDefault(Invoke(this,&LDAFBaseTest::invokeUrlCallbackObjectFunc));
    }

    firstLDAFBase->processAllForward();

    EXPECT_TRUE(mockQObject.getResponseData().size() == urlList.size());


   for (int i =0; i< urlList.size();++i){
        EXPECT_TRUE(mockQObject.getResponseData()[i]==urlList[i]);
    }
}

TEST_F(LDAFBaseTest,LDAFBaseCallBackObjectFunctionCallJsonValidation){
    connectLDAFBases();

    MockQObject mockQObject; 
    //testCallbackFunction is name of slot defined in MockQObject
    LDAFCallBackObject callBackObject = LDAFCallBackObject(&mockQObject,"testCallBackFunction");

    for (auto json: jsonList){
        firstLDAFBase->addCommand(json,callBackObject);
    }
  
   
    for (auto json: jsonList){
       ON_CALL(*secondLDAFBase,setJsonMessage(json,callBackObject))
        .WillByDefault(Invoke(this,&LDAFBaseTest::setResponseJsonMessage));
        
        EXPECT_CALL(*secondLDAFBase,setJsonMessage(json,callBackObject)).Times(1);
        EXPECT_CALL(*firstLDAFBase,setJsonMessage(json,callBackObject)).Times(1); 

      ON_CALL(*firstLDAFBase,setJsonMessage(json,callBackObject))
        .WillByDefault(Invoke(this,&LDAFBaseTest::invokeJsonCallbackObjectFunc));
    }

    firstLDAFBase->processAllForward();

    EXPECT_TRUE(mockQObject.getResponseData().size() == jsonList.size());

   for (int i =0; i< jsonList.size();++i){
        EXPECT_TRUE(mockQObject.getResponseData()[i]==jsonList[i]);
    }
    
}
