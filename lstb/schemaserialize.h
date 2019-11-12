#pragma once
#include <sstream>
#include <map> 
#include <vector>
#include <string>
#include <iostream>

namespace schemaSerialize {


	class Value;
	enum class primitiveType {
		TYPEVALUE, OBJECT, ARRAY, STRING, UINT32, NUMBER, BOOLEAN, VALUE
	};
	class Value {
	public:
		primitiveType type;
		Value(primitiveType ty) :type(ty) {};
		virtual void _serialize(std::ostringstream& strm, Value& schemaPrototype) {};
		virtual std::string asStr();
		virtual ~Value() {
			//std::cout << "dest";
		}
	};

	class primitiveTypeValue :public Value {
	public:
		primitiveTypeValue(primitiveType pt);
		primitiveType thisType;
		std::string asStr() override {
			std::string ast[] = { "TYPEVALUE", "OBJECT", "ARRAY", "STRING", "UINT32", "NUMBER", "BOOLEAN", "VALUE" };
			return("primitiveTypeValue(" + ast[static_cast<int>(thisType)] + ")");
		};
		virtual ~primitiveTypeValue() {
			//std::cout << "dest2";
		}
	};



	class Schema {
	public:
		Schema(std::unique_ptr<schemaSerialize::Value> pt);//pt must be only object or type


		std::string serialize(std::unique_ptr<Value> &inp) ;
		std::unique_ptr<Value> deserialize(std::string inp);
	private:
		std::unique_ptr<schemaSerialize::Value> prototype;
	};

	std::string inline replaceAll(std::string inp, std::string from, std::string to)
	{
		std::string res = inp;
		size_t pos = res.find(from);

		while (pos != std::string::npos)
		{
			res.replace(pos, from.size(), to);
			pos = res.find(from, pos + to.size());

		}
		return(res);
	}

	class Object :public Value {
	public:
		Object();
		std::map< std::string, std::unique_ptr<Value>> attrbts;


		void _serialize(std::ostringstream& strm, Value& schemaPrototype) override;
		static std::unique_ptr<Value> _deserialize(std::istringstream& strm, Value& schemaPrototype);
		std::string asStr() override {
			std::string ret = "{\n";

			for (auto const& x : attrbts)
			{
				ret += "  " + x.first + ":" + replaceAll(x.second.get()->asStr(), "\n", "\n  ") + ",\n";
			}
			return(ret + "}");
		}
	};
	class Array :public Value {
	public:
		Array();
		std::vector< std::unique_ptr<Value>> val;
		void _serialize(std::ostringstream& strm, Value& schemaPrototype) override;
		static std::unique_ptr<Value> _deserialize(std::istringstream& strm, Value& schemaPrototype);
		std::string asStr() override {
			std::string ret = "[\n";

			for (int i = 0; i < val.size(); i++)
			{
				ret += "  " + replaceAll((val[i].get())->asStr(), "\n", "\n  ") + ",\n";
			}
			return(ret + "]");
		}
	};
	class String :public Value {
	public:

		String(std::string str);
		std::string val;
		virtual void _serialize(std::ostringstream& strm, Value& schemaPrototype) override;
		static std::unique_ptr<Value> _deserialize(std::istringstream& strm);
		std::string asStr() override {
			return("\"" + val + "\"");
		}
		~String() {
			//std::cout << "dest";
		}
	};


	class Uint32 :public Value {
	public:

		Uint32(uint32_t num);
		uint32_t val;
		virtual void _serialize(std::ostringstream& strm, Value& schemaPrototype) override;
		static std::unique_ptr<Value> _deserialize(std::istringstream& strm);
		std::string asStr() override {
			return(std::to_string(val));
		}
	};


	class Number :public Value {
	public:

		Number(double num);
		double val;
		void _serialize(std::ostringstream& strm, Value& schemaPrototype) override;
		static std::unique_ptr<Value> _deserialize(std::istringstream& strm);
		std::string asStr() override {
			return(std::to_string(val));

		}
	};


	class Boolean :public Value {
	public:

		Boolean(bool num);
		bool val;
		void _serialize(std::ostringstream& strm, Value& schemaPrototype) override;
		static std::unique_ptr<Value> _deserialize(std::istringstream& strm);
		std::string asStr() override {
			return(val ? "true" : "false");
		}
	};


	//		TYPEVALUE, OBJECT, ARRAY, STRING, UINT32, NUMBER, BOOLEAN

	namespace factory {
		std::unique_ptr<Value> createPrimitiveTypeValue(primitiveType pt);
		std::unique_ptr<Value> createObject(std::map< std::string, std::unique_ptr<Value>>& mp);
		std::unique_ptr<Value> createObject();
		std::unique_ptr<Value> createArray(std::vector<std::unique_ptr<Value>>& vec);
		std::unique_ptr<Value> createString(std::string str);
		std::unique_ptr<Value> createUint32(uint32_t uint32);
		std::unique_ptr<Value> createNumber(double db);
		std::unique_ptr<Value> createBoolean(bool bol);

	}

}