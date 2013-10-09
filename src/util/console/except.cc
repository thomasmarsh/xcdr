#include "common/common.h"

XCDR_NAMESPACE_BEGIN

#define NSTR(x) ((x) ? (x) : string("<null>"))

#if 0
inline std::string icu2str(UnicodeString &in)
{
	static char out[200];
	out[in.extract(0, 99, out)] = 0;
	return string(out);
}
#endif

inline void dump_ident_list(std::string &msg, IdentifierList &l)
{
	if (!l.count()) return;

	IdentifierList::iterator i = l.begin();
	while (i != l.end())
	{
		Identifier *ident = *i;
		msg += "\t\t(" + Util::itoa(ident->i_type) + ", \"";
		msg += NSTR(ident->value) + "\")\n";
		i++;
	}
}

inline void dump_dest_info(std::string &msg, DestinationInfo &d)
{
	msg += "\t\tis_nanp    = " + Util::itoa(d.is_nanp) + "\n";
	msg += "\t\tvalid      = " + Util::itoa(d.valid) + "\n";
	msg += "\t\tcountry    = " + NSTR(d.country) + "\n";
	msg += "\t\tprovince   = " + NSTR(d.province) + "\n";
	msg += "\t\tcity       = " + NSTR(d.city) + "\n";
	msg += "\t\ttimezone   = " + NSTR(d.timezone) + "\n";
	msg += "\t\tlata       = " + NSTR(d.lata) + "\n";
	msg += "\t\tocn        = " + NSTR(d.ocn) + "\n";
}

std::string RatingFailure::dump()
{
	ENTER();
//	DateFormat *dfmt = DateFormat::createInstance();
//	UnicodeString start_time, stop_time;

//	dfmt->setCalendar(*(query->calendar));
//	dfmt->setTimeZone(*(query->timezone));

//	start_time = dfmt->format(query->effective_start, start_time);
//	stop_time = dfmt->format(query->effective_end, stop_time);

	std::string msg("Query:\n");
	msg += "\tcli        = \"" + NSTR(query->cli) + "\"\n";
	msg += "\tcld        = \"" + NSTR(query->cld) + "\"\n";
	msg += "\tpayphone   = " + Util::itoa(query->payphone) + "\n";
	msg += "\tprison     = " + Util::itoa(query->prison) + "\n";
//	msg += "\tstart      = " + icu2str(start_time) + "\n";
//	msg += "\tstop       = " + icu2str(stop_time) + "\n";
	msg += "\tduration   = " + Util::itoa(query->duration) + "\n";
//	msg += "\tpartyid    = " + Util::itox((int) query->party_ident, 8) + "\n";
	msg += "\tidentifiers = {\n";
	dump_ident_list(msg, query->identifiers);
	msg += "\t}\n";

	msg += "\nResponse:\n";
	msg += "\tprice      = " + Util::dtoa(response->charged_amount) + "\n";
	msg += "\ttime       = " + Util::itoa(response->charged_time) + "\n";
	msg += "\ttime_del   = 0\n";
	msg += "\tcli_info   = {\n";
	dump_dest_info(msg, response->cli_info);
	msg += "\t}\n\tcld_info = {\n";
	dump_dest_info(msg, response->cld_info);
	msg += "\t}\n";
	msg += "\ti_customer = " + Util::itoa(response->i_customer) + "\n";
	msg += "\ti_vendor   = " + Util::itoa(response->i_vendor) + "\n";
	msg += "\ti_tariff   = " + Util::itoa(response->i_tariff) + "\n";
	msg += "\ti_activity = " + Util::itoa(response->i_activity) + "\n";
	msg += "\ti_period   = " + Util::itoa(response->i_period) + "\n";
	msg += "\ti_bracket  = " + Util::itoa(response->i_bracket) + "\n";
	msg += "\ti_bracketx = " + Util::itoa(response->i_bracket_index) + "\n";
	msg += "\ti_interval = " + Util::itoa(response->i_interval) + "\n";
	LEAVE();
	return msg;
}

XCDR_NAMESPACE_END
