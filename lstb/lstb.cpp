
#include <iostream>
#include "schemaserialize.h"
int main()
{


	auto gen_random = [](const int len, unsigned int seed) {
		srand(seed);
		std::string ret(len, ' ');
		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		for (int i = 0; i < len; ++i) {
			ret[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
		}

		return(ret);
	};
	for (int i = 0; i < 1000;i++) {
		std::unique_ptr<schemaSerialize::Value> sch = schemaSerialize::factory::createObject();
		{
			{
				auto a = schemaSerialize::factory::createPrimitiveTypeValue(schemaSerialize::primitiveType::STRING);
				dynamic_cast<schemaSerialize::Object*>(sch.get())->attrbts["aString"] = move(a);
			}
			{
				auto k = schemaSerialize::factory::createPrimitiveTypeValue(schemaSerialize::primitiveType::STRING);

				std::vector< std::unique_ptr < schemaSerialize::Value >> a;
				a.push_back(move(k));
				std::unique_ptr<schemaSerialize::Value> b = schemaSerialize::factory::createArray(a);
				dynamic_cast<schemaSerialize::Object*>(sch.get())->attrbts["someStrings"] = move(b);
			}
			{
				auto o = schemaSerialize::factory::createObject();
				auto a = schemaSerialize::factory::createPrimitiveTypeValue(schemaSerialize::primitiveType::STRING);
				dynamic_cast<schemaSerialize::Object*>(o.get())->attrbts["name"] = move(a);
				auto b = schemaSerialize::factory::createPrimitiveTypeValue(schemaSerialize::primitiveType::NUMBER);
				dynamic_cast<schemaSerialize::Object*>(o.get())->attrbts["age"] = move(b);
				auto c = schemaSerialize::factory::createPrimitiveTypeValue(schemaSerialize::primitiveType::UINT32);
				dynamic_cast<schemaSerialize::Object*>(o.get())->attrbts["id"] = move(c);
				auto d = schemaSerialize::factory::createPrimitiveTypeValue(schemaSerialize::primitiveType::BOOLEAN);
				dynamic_cast<schemaSerialize::Object*>(o.get())->attrbts["isdead"] = move(d);

				std::vector< std::unique_ptr < schemaSerialize::Value >> e;
				e.push_back(move(o));
				std::unique_ptr<schemaSerialize::Value> f = schemaSerialize::factory::createArray(e);
				dynamic_cast<schemaSerialize::Object*>(sch.get())->attrbts["somePeople"] = move(f);
			}
		}

		schemaSerialize::Schema schema(move(sch));


		//std::cout << "Schema:\n" << schema->asStr() << "\n\n";


		std::unique_ptr<schemaSerialize::Value> obj = schemaSerialize::factory::createObject();
		//dynamic_cast<schemaSerialize::Object*>(obj.get())->attrbts["aString"] = schemaSerialize::factory::createString("helloWorld");
		{
			{
				auto a = schemaSerialize::factory::createString("hehe");
				dynamic_cast<schemaSerialize::Object*>(obj.get())->attrbts["aString"] = move(a);
			}
			{

				std::vector< std::unique_ptr < schemaSerialize::Value >> a;
				a.push_back(move(schemaSerialize::factory::createString("hey")));
				a.push_back(move(schemaSerialize::factory::createString("does")));
				a.push_back(move(schemaSerialize::factory::createString("this")));
				a.push_back(move(schemaSerialize::factory::createString("work")));
				a.push_back(move(schemaSerialize::factory::createString("??/")));

				std::unique_ptr<schemaSerialize::Value> b = schemaSerialize::factory::createArray(a);
				dynamic_cast<schemaSerialize::Object*>(obj.get())->attrbts["someStrings"] = move(b);
			}
			{


				std::string df = gen_random(10000, 41);
				std::vector< std::unique_ptr < schemaSerialize::Value >> e;
				for (int k = 0; k < 10000; k++) {
					auto o = schemaSerialize::factory::createObject();
					auto a = schemaSerialize::factory::createString(df);
					dynamic_cast<schemaSerialize::Object*>(o.get())->attrbts["name"] = move(a);
					auto b = schemaSerialize::factory::createNumber(k * 3.6 + 12.2);
					dynamic_cast<schemaSerialize::Object*>(o.get())->attrbts["age"] = move(b);
					auto c = schemaSerialize::factory::createUint32(k + 83400);
					dynamic_cast<schemaSerialize::Object*>(o.get())->attrbts["id"] = move(c);
					auto d = schemaSerialize::factory::createBoolean(k % 2);
					dynamic_cast<schemaSerialize::Object*>(o.get())->attrbts["isdead"] = move(d);
					e.push_back(move(o));
				}
				std::unique_ptr<schemaSerialize::Value> f = schemaSerialize::factory::createArray(e);
				dynamic_cast<schemaSerialize::Object*>(obj.get())->attrbts["somePeople"] = move(f);
			}
		}
		//std::cout << "Before Serialization:\n" << obj->asStr() << "\n\n";
		std::string serialied = schema.serialize(obj);


		std::unique_ptr<schemaSerialize::Value> deserialied = schema.deserialize(serialied);

		std::string reserialied = schema.serialize(deserialied);


		//if (serialied == reserialied)std::cout << "ok\n";
		continue;
		/*
		//std::cout << "After Serialization:\n" << obj2->asStr() << "\n\n";
		int c = 0;
		for (auto& k : dynamic_cast<schemaSerialize::Array*>(dynamic_cast<schemaSerialize::Object*>(obj2.get())->attrbts["somePeople"].get())->val) {
			std::string st = dynamic_cast<schemaSerialize::String*>((dynamic_cast<schemaSerialize::Object*>(k.get()))->attrbts["name"].get())->val;
			if (st != gen_random(10000, c)) {
				//std::cout << "There is a prob\n";
				break;
			}
			c++;
		}*/
		//std::cout << obj;
	}
}
 
