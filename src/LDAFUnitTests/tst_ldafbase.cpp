#include "tst_ldaf.h"

using ::testing::AtLeast;
TEST(LDAFMessageTypeTest, LDAFMessageTypeConstruction){
    MockLDAFBase mockLDAFbase;
    MockQObject mockQObject;
    MockLDAFMessageType ldafMessage(&mockLDAFbase,&mockQObject,"testFunction");
    EXPECT_EQ(ldafMessage.getBasicObject(),&mockLDAFbase);
    EXPECT_EQ(ldafMessage.getCallBackObject(),&mockQObject);
    EXPECT_EQ(ldafMessage.getCallBackJSFunc(),"testFunction");    
}