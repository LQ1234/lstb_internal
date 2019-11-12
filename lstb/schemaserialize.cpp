#include "schemaserialize.h"
 schemaSerialize::Schema::Schema(std::unique_ptr<schemaSerialize::Value> pt) {
	prototype = move(pt);
}

 std::string schemaSerialize::Schema::serialize(std::unique_ptr<Value>& inp)
 {
	 std::ostringstream ss;

	 inp.get()->_serialize(ss, *prototype.get());
	 return ss.str();
 }

 std::unique_ptr<schemaSerialize::Value> schemaSerialize::Schema::deserialize(std::string inp)
 {
	 std::istringstream ss(inp);

	 if (prototype.get()->type == primitiveType::OBJECT) {
		 return(Object::_deserialize(ss, *prototype.get()));

	 }
	 else if (prototype.get()->type == primitiveType::ARRAY) {
		 return(Array::_deserialize(ss, *prototype.get()));
	 }
	 else if (prototype.get()->type == primitiveType::TYPEVALUE) {
		 Value* val;
		 switch (dynamic_cast<primitiveTypeValue*>(prototype.get())->thisType) {

		 case primitiveType::STRING:
			 return(String::_deserialize(ss));
			 break;
		 case primitiveType::UINT32:
			 return(Uint32::_deserialize(ss));
			 break;
		 case primitiveType::NUMBER:
			 return(Number::_deserialize(ss));
			 break;
		 case primitiveType::BOOLEAN:
			 return(Boolean::_deserialize(ss));
			 break;

		 }
	 }
 }



schemaSerialize::Object::Object():Value(primitiveType::OBJECT)
{

}

void schemaSerialize::Object::_serialize(std::ostringstream& strm, Value& schemaPrototype) {//schemaPrototype is expected to be Object
	Object& schemaPrototypeObj = dynamic_cast< Object&>(schemaPrototype);
	 
	for (auto const& x : schemaPrototypeObj.attrbts)
	{	
		attrbts[x.first].get()->_serialize(strm,*(x.second.get()));
		//x.second.get()->serialize(strm, (*schemaPrototypeObj.attrbts.find(x.first)->second.get()));
	}
}

 std::unique_ptr<schemaSerialize::Value> schemaSerialize::Object::_deserialize(std::istringstream& strm, Value& schemaPrototype) {
	Object* newObj = new Object();

	Object& schemaPrototypeObj = dynamic_cast<Object&>(schemaPrototype);

	for (auto const& x : schemaPrototypeObj.attrbts)
	{	

		if (x.second.get()->type== primitiveType::OBJECT) {
			newObj->attrbts[x.first] = Object::_deserialize(strm,*x.second.get());

		}
		else if (x.second.get()->type == primitiveType::ARRAY) {
			newObj->attrbts[x.first] = Array::_deserialize(strm, *x.second.get());
		}
		else if (x.second.get()->type == primitiveType::TYPEVALUE) {
			Value* val;
			switch (dynamic_cast<primitiveTypeValue*>(x.second.get())->thisType) {

			case primitiveType::STRING:
				newObj->attrbts[x.first] = String::_deserialize(strm);
				break;
			case primitiveType::UINT32:
				newObj->attrbts[x.first] = Uint32::_deserialize(strm);
				break;
			case primitiveType::NUMBER:
				newObj->attrbts[x.first] = Number::_deserialize(strm);
				break;
			case primitiveType::BOOLEAN:
				newObj->attrbts[x.first] = Boolean::_deserialize(strm);

				break;

			}
		}

		//newObj->attrbts[x.first]=x.second::
	}
	return(std::unique_ptr<Value>(newObj));
}



 schemaSerialize::Array::Array():Value(primitiveType::ARRAY)
 {
 }

 void schemaSerialize::Array::_serialize(std::ostringstream& strm, Value& schemaPrototype) {
	uint32_t len = static_cast<uint32_t>(val.size());
	strm.write(reinterpret_cast<char*>(&len), 4);

	Array& schemaPrototypeArr = dynamic_cast<Array&>(schemaPrototype);

	for (int i = 0; i < val.size(); i++) {
		val[i]->_serialize(strm, *(schemaPrototypeArr.val[0].get()));
	}

}

 std::unique_ptr<schemaSerialize::Value> schemaSerialize::Array::_deserialize(std::istringstream& strm, Value& schemaPrototype) {
	Array* newArr = new Array();

	char lenarr[4];
	strm.read(lenarr, 4);
	uint32_t len = *reinterpret_cast<uint32_t*>(lenarr);
	newArr->val.resize(len);

	Array& schemaPrototypeArr = dynamic_cast<Array&>(schemaPrototype);

	for (int i=0;i<len;i++)
	{
		if (schemaPrototypeArr.val[0].get()->type == primitiveType::OBJECT) {
			newArr->val[i] = Object::_deserialize(strm, *schemaPrototypeArr.val[0].get());
		}
		else if (schemaPrototypeArr.val[0].get()->type == primitiveType::ARRAY) {
			newArr->val[i] = Array::_deserialize(strm, *schemaPrototypeArr.val[0].get());
		}
		else if (schemaPrototypeArr.val[0].get()->type == primitiveType::TYPEVALUE) {
			Value* val;
			switch (dynamic_cast<primitiveTypeValue*>(schemaPrototypeArr.val[0].get())->thisType) {

			case primitiveType::STRING:
				newArr->val[i] = String::_deserialize(strm);
				break;
			case primitiveType::UINT32:
				newArr->val[i] = Uint32::_deserialize(strm);
				break;
			case primitiveType::NUMBER:
				newArr->val[i] = Number::_deserialize(strm);
				break;
			case primitiveType::BOOLEAN:
				newArr->val[i] = Boolean::_deserialize(strm);

				break;

			}
		}
	}
	return(std::unique_ptr<Value>(newArr));

}

 schemaSerialize::String::String(std::string str):Value(primitiveType::STRING) {
	val = str;
}

 void schemaSerialize::String::_serialize(std::ostringstream& strm, Value& schemaPrototype) {
	uint32_t len = static_cast<uint32_t>(val.size());
	strm.write(reinterpret_cast<char*>(&len), 4);
	strm.write(val.c_str(), val.size());
}

 std::unique_ptr<schemaSerialize::Value> schemaSerialize::String::_deserialize(std::istringstream& strm) {
	char lenarr[4];
	strm.read(lenarr, 4);
	uint32_t len = *reinterpret_cast<uint32_t*>(lenarr);
	char* arrchar = new char[len];
	strm.read(arrchar, len);
	std::string asStr = std::string(arrchar,len);
	delete[] arrchar;
	return(std::unique_ptr<Value>(new String(asStr)));
}

 schemaSerialize::Uint32::Uint32(uint32_t num) :Value(primitiveType::UINT32) {
	val = num;
}

 void schemaSerialize::Uint32::_serialize(std::ostringstream& strm, Value& schemaPrototype) {
	strm.write(reinterpret_cast<char*>(&val), 4);
}

 std::unique_ptr<schemaSerialize::Value> schemaSerialize::Uint32::_deserialize(std::istringstream& strm) {
	char datarr[4];
	strm.read(datarr, 4);
	uint32_t rval = *reinterpret_cast<uint32_t*>(datarr);
	return(std::unique_ptr<Value>(new Uint32(rval)));
}

 schemaSerialize::Number::Number(double num) :Value(primitiveType::NUMBER) {
	val = num;
}

 void schemaSerialize::Number::_serialize(std::ostringstream& strm, Value& schemaPrototype) {
	strm.write(reinterpret_cast<char*>(&val), 8);
}

 std::unique_ptr<schemaSerialize::Value> schemaSerialize::Number::_deserialize(std::istringstream& strm) {
	char datarr[8];
	strm.read(datarr, 8);
	double rval = *reinterpret_cast<double*> (datarr);
	return(std::unique_ptr<Value>(new Number(rval)));
}

 schemaSerialize::Boolean::Boolean(bool num) :Value(primitiveType::BOOLEAN) {
	val = num;
}

 void schemaSerialize::Boolean::_serialize(std::ostringstream& strm, Value& schemaPrototype) {
	strm.write(reinterpret_cast<char*>(&val), 1);
}

 std::unique_ptr<schemaSerialize::Value> schemaSerialize::Boolean::_deserialize(std::istringstream& strm) {
	char datarr[1];
	strm.read(datarr, 1);
	bool rval = *reinterpret_cast<bool*> (datarr);
	return(std::unique_ptr<Value>(new Boolean(rval)));
}

 schemaSerialize::primitiveTypeValue::primitiveTypeValue(primitiveType pt) :Value(primitiveType::TYPEVALUE) {
	thisType = pt;
}

std::unique_ptr<schemaSerialize::Value> schemaSerialize::factory::createPrimitiveTypeValue(primitiveType pt)
{
	return std::unique_ptr<Value>(new primitiveTypeValue(pt));
}

std::unique_ptr<schemaSerialize::Value> schemaSerialize::factory::createObject(std::map<std::string, std::unique_ptr<Value>>& mp) {
	Object* obj = new Object();
	obj->attrbts = move(mp);
	return(std::unique_ptr<Value>(obj));
}

std::unique_ptr<schemaSerialize::Value> schemaSerialize::factory::createObject()
{
	return std::unique_ptr<Value>(new Object());
}

std::unique_ptr<schemaSerialize::Value> schemaSerialize::factory::createArray(std::vector<std::unique_ptr<Value>>& vec)
{
	Array* arr = new Array();
	arr->val = move(vec);
	return(std::unique_ptr<Value>(arr));
}

std::unique_ptr<schemaSerialize::Value> schemaSerialize::factory::createString(std::string str)
{
	return std::unique_ptr<Value>(new String(str));
}

std::unique_ptr<schemaSerialize::Value> schemaSerialize::factory::createUint32(uint32_t uint32)
{
	return std::unique_ptr<Value>(new Uint32(uint32));
}

std::unique_ptr<schemaSerialize::Value> schemaSerialize::factory::createNumber(double db)
{
	return std::unique_ptr<Value>(new Number(db));
}

std::unique_ptr<schemaSerialize::Value> schemaSerialize::factory::createBoolean(bool bol)
{
	return std::unique_ptr<Value>(new Boolean(bol));
}

std::string schemaSerialize::Value::asStr()
{
	return "Value";
}
