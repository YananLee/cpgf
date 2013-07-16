#include "buildersection.h"
#include "codewriter/codeblock.h"
#include "codewriter/codewriter.h"
#include "util.h"

namespace metagen {


BuilderSection::BuilderSection(BuilderSectionType type, const CppItem * cppItem)
	: type(type), cppItem(cppItem), totalPayload(0), index(0), codeBlock(new CodeBlock)
{
}

CodeBlock * BuilderSection::getCodeBlock() const
{
	return this->codeBlock.get();
}

BuilderSectionType BuilderSection::getType() const
{
	return this->type;
}

void BuilderSection::addPayload(size_t payload)
{
	this->totalPayload += payload;
}

size_t BuilderSection::getTotalPayload() const
{
	return this->totalPayload;
}

void BuilderSection::setIndex(size_t index)
{
	this->index = index;
}

size_t BuilderSection::getIndex() const
{
	return this->index;
}


BuilderSectionList::BuilderSectionList()
{
}

BuilderSectionList::~BuilderSectionList()
{
	clearPointerContainer(this->sectionList);
}

BuilderSection * BuilderSectionList::addSection(BuilderSectionType type, const CppItem * cppItem)
{
	BuilderSection * section = new BuilderSection(type, cppItem);
	this->sectionList.push_back(section);
	return section;
}

void BuilderSectionList::dump()
{
	printf("Section list begin \n");
	for(BuilderSectionList::iterator it = this->begin(); it != this->end(); ++it) {
		CodeWriter codeWriter;
		(*it)->getCodeBlock()->write(&codeWriter);
		printf("%s\n\n", codeWriter.getText().c_str());
	}
	printf("Section list end \n");
}


} // namespace metagen

