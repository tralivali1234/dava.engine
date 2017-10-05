#pragma once

#include "Base/FastName.h"

#include "TArc/Core/ContextAccessor.h"
#include "TArc/DataProcessing/Common.h"

#include <Reflection/Reflection.h>

namespace DAVA
{
class ReflectedType;
namespace TArc
{
class ContextAccessor;
class FieldBinder final
{
public:
    FieldBinder(ContextAccessor* accessor);
    ~FieldBinder();

    void BindField(const FieldDescriptor& fieldDescr, const Function<void(const Any&)>& fn);
    void BindField(const Reflection& model, const FastName& fieldName, const Function<void(const Any&)>& fn);
    void SetValue(const FieldDescriptor& fieldDescr, const Any& v);

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};
} // namespace TArc
} // namespace DAVA
