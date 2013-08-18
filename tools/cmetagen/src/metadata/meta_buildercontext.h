/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_BuilderContext
@@cpgf@creations@*/

#ifndef METADATA_META_BUILDERCONTEXT_H
#define METADATA_META_BUILDERCONTEXT_H

#include "buildermodel/buildercontext.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gselectFunctionByArity.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"

namespace metadata {

template <typename D_d >
void buildMetaClass_metagen_BuilderContext(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    

    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::Project *, const std::basic_string<char, std::char_traits<char>, std::allocator<char> > &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<1> >());

    _d.CPGF_MD_TEMPLATE _method("process", &D_d::ClassType::process);
    _d.CPGF_MD_TEMPLATE _method("getProject", &D_d::ClassType::getProject);
    _d.CPGF_MD_TEMPLATE _method("getSourceBaseFileName", &D_d::ClassType::getSourceBaseFileName), cpgf::MakePolicy<GMetaRuleCopyConstReference<-1> >();
    _d.CPGF_MD_TEMPLATE _method("getItemList", &D_d::ClassType::getItemList);
    _d.CPGF_MD_TEMPLATE _method("getSectionList", &D_d::ClassType::getSectionList);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderContext();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
