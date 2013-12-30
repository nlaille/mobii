#pragma once


/**
 * \file Model.h
 * \brief Declaration of Models used by database
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <string>
#include <soci.h>

/**
 * \struct SmartPhone
 * \brief Representation of Database Smartphones
 */
struct SmartPhone
{
	std::string	uid;
	std::string imei;
	std::string model;
	std::string manufacturer;

	inline SmartPhone&	operator<<(const soci::row& row)
	{
		row >> uid >> imei >> model >> manufacturer;
		return *this;
	}
};

/**
 * \struct SmartPhoneName
 * \brief Representation of Database Smartphones
 */
struct SmartPhoneName
{
	std::string	uid;
	std::string	name;
	std::string imei;
	std::string model;
	std::string manufacturer;

	inline SmartPhoneName&	operator<<(const soci::row& row)
	{
		row >> uid >> name >> imei >> model >> manufacturer;
		return *this;
	}
};

/**
 * \struct User
 * \brief Representation of Database Users
 */
struct User
{
	int id;
	std::string email;
	std::string	password_md5_digest;

	inline User&	operator<<(const soci::row& row)
	{
		row >> id >> email >> password_md5_digest;
		return *this;
	}
};

/**
 * \struct Uid
 * \brief Representation of Database Uids
 */
struct Uid
{
	std::string	uid;
	int	password;

	inline Uid&		operator<<(const soci::row& row)
	{
		row >> uid >> password;
		return *this;
	}
};

/**
 * \struct UserSmartPhone
 * \brief Representation of Database UserSmartPhones
 */
struct UserSmartPhone
{
	std::string	name;
	std::string	uidSmartPhone;
	int	userId;
};

namespace soci
{
    template<>
    struct type_conversion<SmartPhone>
    {
        typedef values base_type;

        static void from_base(values const & values, indicator /* ind */, SmartPhone& smartPhone)
        {
            smartPhone.uid = values.get<std::string>("uid");
			smartPhone.imei = values.get<std::string>("imei");
			smartPhone.model = values.get<std::string>("model");
			smartPhone.manufacturer = values.get<std::string>("manufacturer");
        }
    
        static void to_base(const SmartPhone& smartPhone, values & values, indicator & ind)
        {
            values.set("uid", smartPhone.uid);
			values.set("imei", smartPhone.imei);
			values.set("model", smartPhone.model);
			values.set("manufacturer", smartPhone.manufacturer);
            ind = i_ok;
        }
    };

	template<>
    struct type_conversion<SmartPhoneName>
    {
        typedef values base_type;

        static void from_base(values const & values, indicator /* ind */, SmartPhoneName& smartPhoneName)
        {
            smartPhoneName.uid = values.get<std::string>("uid");
			smartPhoneName.name = values.get<std::string>("name");
			smartPhoneName.imei = values.get<std::string>("imei");
			smartPhoneName.model = values.get<std::string>("model");
			smartPhoneName.manufacturer = values.get<std::string>("manufacturer");
        }
    
        static void to_base(const SmartPhoneName& smartPhoneName, values & values, indicator & ind)
        {
            values.set("uid", smartPhoneName.uid);
			values.set("name", smartPhoneName.name);
			values.set("imei", smartPhoneName.imei);
			values.set("model", smartPhoneName.model);
			values.set("manufacturer", smartPhoneName.manufacturer);
            ind = i_ok;
        }
    };

	template<>
    struct type_conversion<User>
    {
        typedef values base_type;

        static void from_base(values const & values, indicator /* ind */, User& user)
        {
            user.id = values.get<int>("id");
            user.email = values.get<std::string>("email");
			user.password_md5_digest = values.get<std::string>("password_md5_digest");
        }
    
        static void to_base(const User& user, values & values, indicator & ind)
        {
            values.set("id", user.id);
            values.set("email", user.email);
			values.set("password_md5_digest", user.password_md5_digest);
            ind = i_ok;
        }
    };

	template<>
    struct type_conversion<Uid>
    {
        typedef values base_type;

        static void from_base(values const & values, indicator /* ind */, Uid& uid)
        {
            uid.uid = values.get<std::string>("uid");
            uid.password = values.get<int>("password");
        }
    
        static void to_base(const Uid& uid, values & values, indicator & ind)
        {
            values.set("uid", uid.uid);
            values.set("password", uid.password);
            ind = i_ok;
        }
    };

	template<>
    struct type_conversion<UserSmartPhone>
    {
        typedef values base_type;
    
		static void from_base(values const & values, indicator /* ind */, UserSmartPhone& userSmartPhone)
        {
            userSmartPhone.name = values.get<std::string>("name");
            userSmartPhone.userId = values.get<int>("user_id");
			userSmartPhone.uidSmartPhone = values.get<std::string>("uid");
        }

        static void to_base(const UserSmartPhone& userSmartPhone, values & values, indicator & ind)
        {
            values.set("name", userSmartPhone.name);
            values.set("user_id", userSmartPhone.userId);
			values.set("uid", userSmartPhone.uidSmartPhone);
            ind = i_ok;
        }
    };
}