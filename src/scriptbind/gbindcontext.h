#ifndef GBINDCONTEXT_H
#define GBINDCONTEXT_H

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptservice.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gstringmap.h"
#include "cpgf/glifecycle.h"

#include "gbindgluedata.h"

#include <vector>
#include <tuple>
#include <map>
#include <memory>
#include <string>

namespace cpgf {

namespace bind_internal {

class GBindingContext;
class GClassPool;
class GScriptObjectCache;

class GScriptContext : public IScriptContext
{
public:
	explicit GScriptContext(GBindingContext * bindingContext) : bindingContext(bindingContext) {}
	virtual ~GScriptContext() {}

private:
	typedef GSharedInterface<IScriptUserConverter> ScriptUserConverterType;
	typedef std::vector<ScriptUserConverterType> ScriptUserConverterListType;

	G_INTERFACE_IMPL_OBJECT

protected:
	virtual void G_API_CC addScriptUserConverter(IScriptUserConverter * converter) override;
	virtual void G_API_CC removeScriptUserConverter(IScriptUserConverter * converter) override;
	virtual uint32_t G_API_CC getScriptUserConverterCount() override;
	virtual IScriptUserConverter * G_API_CC getScriptUserConverterAt(uint32_t index) override;
	virtual void G_API_CC setAllowGC(const GVariantData * instance, gapi_bool allowGC) override;
	virtual void G_API_CC bindExternalObjectToClass(void * address, IMetaClass * metaClass) override;

private:
	ScriptUserConverterListType::iterator findConverter(IScriptUserConverter * converter);

private:
	GScopedPointer<ScriptUserConverterListType> scriptUserConverterList;
	GBindingContext * bindingContext;
	std::vector<GObjectGlueDataPointer> externalObjects;
};

class GBindingPool
{
private:
	typedef std::tuple<void *, void *> MethodKey; // <method, instance>
	
	typedef std::tuple<void *, void *, GScriptInstanceCv, bool> ObjectKey; // <class address, object address, constness, allowGC>

	struct ClassKey
	{
		explicit ClassKey(IMetaClass * metaClass)
			: metaClass(metaClass)
		{}

		IMetaClass * metaClass;

		bool operator < (const ClassKey & other) const {
			return this->metaClass < other.metaClass;
		}
	};

public:
	explicit GBindingPool(const GSharedPointer<GBindingContext> & context);
	~GBindingPool();

	template <typename T>
	void glueDataAdded(const T & /*glueData*/) {}
	template <typename T>
	void glueDataRemoved(const T & /*glueData*/) {}

	void glueDataAdded(const GMethodGlueDataPointer & glueData);
	void glueDataRemoved(const GMethodGlueDataPointer & glueData);
	GMethodGlueDataPointer newMethodGlueData(const GScriptValue & scriptValue);

	void glueDataAdded(const GObjectGlueDataPointer & glueData);
	void glueDataRemoved(const GObjectGlueDataPointer & glueData);
	GObjectGlueDataPointer newObjectGlueData(
		const GClassGlueDataPointer & classData,
		const GObjectInstancePointer & objectInstance,
		const bool allowGC,
		const GScriptInstanceCv cv
	);

	void glueDataAdded(const GObjectAndMethodGlueDataPointer & glueData);
	void glueDataRemoved(const GObjectAndMethodGlueDataPointer & glueData);

	void objectInstanceAdded(const GObjectInstancePointer & objectData);
	void objectInstanceDestroyed(const GObjectInstance * objectData);
	GObjectInstancePointer findObjectInstance(const GVariant & instance);

	GClassGlueDataPointer getClassData(IMetaClass * metaClass);
	void classDestroyed(IMetaClass * metaClass);

private:
	MethodKey doMakeMethodKey(const GScriptValue & scriptValue);

private:
	GWeakPointer<GBindingContext> context;

	std::map<MethodKey, GWeakMethodGlueDataPointer> methodGlueDataMap;
	std::map<ObjectKey, GWeakObjectGlueDataPointer> objectGlueDataMap;
	std::map<void *, GWeakObjectInstancePointer> instanceMap;
	std::map<ClassKey, GClassGlueDataPointer> classMap;
};

class GBindingContext : public GShareFromThis<GBindingContext>
{
public:
	explicit GBindingContext(IMetaService * service);
	virtual ~GBindingContext();

	IMetaService * getService() const {
		return this->service.get();
	}

	void bindScriptCoreService(GScriptObject * scriptObject, const char * bindName, IScriptLibraryLoader * libraryLoader);

	IScriptContext * borrowScriptContext() const;

public:
	GClassGlueDataPointer getClassData(IMetaClass * metaClass);
	GClassGlueDataPointer newClassData(IMetaClass * metaClass);
	void classDestroyed(IMetaClass * metaClass);

	GObjectInstancePointer findObjectInstance(const GVariant & instance);

	GObjectGlueDataPointer newObjectGlueData(
		const GClassGlueDataPointer & classData,
		const GVariant & instance,
		const bool allowGC,
		const GScriptInstanceCv cv
	);

	GMethodGlueDataPointer newMethodGlueData(const GScriptValue & scriptValue);

	GEnumGlueDataPointer newEnumGlueData(IMetaEnum * metaEnum);

	GAccessibleGlueDataPointer newAccessibleGlueData(void * instance, IMetaAccessible * accessible);

	GRawGlueDataPointer newRawGlueData(const GVariant & data);

	GObjectAndMethodGlueDataPointer newObjectAndMethodGlueData(
		const GObjectGlueDataPointer & objectData,
		const GMethodGlueDataPointer & methodData
	);

	GOperatorGlueDataPointer newOperatorGlueData(
		const GObjectGlueDataPointer & objectData,
		IMetaClass * metaClass,
		GMetaOpType op
	);
	
	GBindingPool * getBindingPool();

	GScriptObjectCache * getScriptObjectCache();

private:
	GClassGlueDataPointer createClassGlueData(IMetaClass * metaClass);

private:
	GSharedInterface<IMetaService> service;
	std::shared_ptr<GBindingPool> bindingPool;

	GScopedPointer<GScriptCoreService> scriptCoreService;
	GScopedInterface<IScriptContext> scriptContext;

	GScopedPointer<GScriptObjectCache> scriptObjectCache;

private:
	template <typename T>
	friend class GGlueDataWrapperImplement;

	friend class GClassGlueData;
	friend class GObjectInstance;
	friend class GObjectGlueData;
	friend class GClassPool;
};


} //namespace bind_internal

} //namespace cpgf


#endif
