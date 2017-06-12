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
    url.setPath("ExampleResourcePath");
    MockLDAFUrl ldafUrl(url,&mockLDAFbase,&mockQObject,"testFunction");
    EXPECT_EQ(ldafUrl.getBasicObject(),&mockLDAFbase);
    EXPECT_EQ(ldafUrl.getCallBackObject(),&mockQObject);
    EXPECT_EQ(ldafUrl.getCallBackJSFunc(),"testFunction");  
    EXPECT_EQ(ldafUrl.getUrl().path(),"ExampleResourcePath");  
}