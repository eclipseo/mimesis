#pragma once

/* Mimesis -- a library for parsing and creating RFC2822 messages
   Copyright © 2017 Guus Sliepen <guus@lightbts.info>
 
   Mimesis is free software; you can redistribute it and/or modify it under the
   terms of the GNU Lesser General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.
 
   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
   more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

namespace Mimesis {

class MIMEPart {
	std::vector<std::pair<std::string, std::string>> headers;
	std::string preamble;
	std::string body;
	std::string epilogue;
	std::vector<MIMEPart> parts;
	std::string boundary;
	bool multipart;
	bool crlf;

	protected:
	bool message;

	public:
	MIMEPart();

	// Loading and saving a whole MIME message
	std::string load(std::istream &in, const std::string &parent_boundary = {});
	void load(const std::string &filename);
	void save(std::ostream &out) const;
	void save(const std::string &filename) const;
	void from_string(const std::string &data);
	std::string to_string() const;

	// Low-level access
	std::string get_body() const;
	std::string get_preamble() const;
	std::string get_epilogue() const;
	std::string get_boundary() const;
	std::vector<MIMEPart> &get_parts();
	const std::vector<MIMEPart> &get_parts() const;
	std::vector<std::pair<std::string, std::string>> &get_headers();
	const std::vector<std::pair<std::string, std::string>> &get_headers() const;
	bool is_multipart() const;

	void set_body(const std::string &body);
	void set_preamble(const std::string &preamble);
	void set_epilogue(const std::string &epilogue);
	void set_boundary(const std::string &boundary);
	void set_parts(const std::vector<MIMEPart> &parts);
	void set_headers(const std::vector<std::pair<std::string, std::string>> &headers);

	void clear();

	// Header manipulation
	std::string get_header(const std::string &field) const;
	void set_header(const std::string &field, const std::string &value);
	std::string &operator[](const std::string &field);
	const std::string &operator[](const std::string &field) const;
	void append_header(const std::string &field, const std::string &value);
	void prepend_header(const std::string &field, const std::string &value);
	void erase_header(const std::string &field);
	void clear_headers();

	// Specialized header functions
	std::string get_multipart_type() const;
	std::string get_header_value(const std::string &field) const;
	std::string get_header_parameter(const std::string &field, const std::string &parameter) const;

	void set_header_value(const std::string &field, const std::string &value);
	void set_header_parameter(const std::string &field, const std::string &paramter, const std::string &value);

	// Part manipulation
	MIMEPart &append_part(const MIMEPart &part = {});
	MIMEPart &prepend_part(const MIMEPart &part = {});
	void clear_parts();
	void make_multipart(const std::string &type, const std::string &boundary = {});
	bool make_singlepart();

	std::string get_mime_type() const;

	// Body and attachments
	MIMEPart &set_alternative(const std::string &subtype, const std::string &text);
	void set_plain(const std::string &text);
	void set_html(const std::string &text);

	const MIMEPart *get_first_matching_part(const std::string &type) const;
	MIMEPart *get_first_matching_part(const std::string &type);
	std::string get_first_matching_body(const std::string &type) const;
	std::string get_text() const;
	std::string get_plain() const;
	std::string get_html() const;

	MIMEPart &attach(const MIMEPart &attachment);
	MIMEPart &attach(const std::string &data, const std::string &mime_type, const std::string &filename = {});
	MIMEPart &attach(std::istream &in, const std::string &mime_type, const std::string &filename = {});
	std::vector<const MIMEPart *> get_attachments() const;

	void clear_text();
	void clear_plain();
	void clear_html();
	void clear_attachments();

	bool has_text() const;
	bool has_plain() const;
	bool has_html() const;
	bool has_attachments() const;

	// Format manipulation
	void set_crlf(bool value = true);
};

class Message: public MIMEPart {
	public:
	Message();
};

}

std::ostream &operator<<(std::ostream &out, const Mimesis::MIMEPart &part) {
	part.save(out);
	return out;
}

std::istream &operator>>(std::istream &in, Mimesis::MIMEPart part) {
	part.load(in);
	return in;
}
