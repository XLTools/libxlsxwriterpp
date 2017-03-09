//  :copyright: (c) 2017 Alex Huszagh.
//  :license: FreeBSD, see LICENSE.md for more details.
/**
 *  C++ bindings for libxlsxwriter workbooks.
 */

#include "workbook.hpp"
#include "utility.hpp"

#include <cassert>


namespace xlsxwriter
{
// OBJECTS
// -------


Options::Options(Options &&other):
    ptr(other.ptr)
{
    other.ptr = nullptr;
}


Options & Options::operator=(Options &&other)
{
    ptr = other.ptr;
    other.ptr = nullptr;
    return *this;
}


Options::~Options()
{
    if (ptr) {
        free(ptr->tmpdir);
        delete ptr;
    }
}


Options::Options(const bool constant_memory):
    ptr(new lxw_workbook_options)
{
    ptr->constant_memory = constant_memory;
    ptr->tmpdir = nullptr;
}


Options::Options(const std::string &tmpdir):
    ptr(new lxw_workbook_options)
{
    ptr->constant_memory = 0;
    ptr->tmpdir = lxw_strdup(tmpdir.data());
}


Options::Options(const bool const_memory,
        const std::string &tmpdir):
    ptr(new lxw_workbook_options)
{
    ptr->constant_memory = const_memory;
    ptr->tmpdir = lxw_strdup(tmpdir.data());
}


Properties::Properties()
{
    ptr = new lxw_doc_properties {nullptr};
}


Properties::Properties(Properties &&other):
    ptr(other.ptr)
{
    other.ptr = nullptr;
}


Properties & Properties::operator=(Properties &&other)
{
    ptr = other.ptr;
    other.ptr = nullptr;
    return *this;
}


Properties::~Properties()
{
    if (ptr) {
        free(ptr->title);
        free(ptr->author);
        free(ptr->manager);
        free(ptr->company);
        free(ptr->category);
        free(ptr->keywords);
        free(ptr->comments);
        free(ptr->status);
        free(ptr->hyperlink_base);
        free(ptr);
    }
}


void Properties::set_title(const std::string &string)
{
    ptr->title = lxw_strdup(string.data());
}


void Properties::set_author(const std::string &string)
{
    ptr->author = lxw_strdup(string.data());
}


void Properties::set_manager(const std::string &string)
{
    ptr->manager = lxw_strdup(string.data());
}


void Properties::set_company(const std::string &string)
{
    ptr->company = lxw_strdup(string.data());
}


void Properties::set_category(const std::string &string)
{
    ptr->category = lxw_strdup(string.data());
}


void Properties::set_keywords(const std::string &string)
{
    ptr->keywords = lxw_strdup(string.data());
}


void Properties::set_comments(const std::string &string)
{
    ptr->comments = lxw_strdup(string.data());
}


void Properties::set_status(const std::string &string)
{
    ptr->status = lxw_strdup(string.data());
}


void Properties::set_hyperlink_base(const std::string &string)
{
    ptr->hyperlink_base = lxw_strdup(string.data());
}


Workbook::Workbook(Workbook &&other):
    ptr(other.ptr)
{
    other.ptr = nullptr;
}


Workbook & Workbook::operator=(Workbook &&other)
{
    ptr = other.ptr;
    other.ptr = nullptr;
    return *this;
}


Workbook::~Workbook()
{
    close();
}


Workbook::Workbook(const std::string &filename,
    const Options &options)
{
    ptr = workbook_new_opt(filename.data(), options.ptr);
}


Worksheet Workbook::add_worksheet(const std::string &sheetname)
{
    assert(ptr && "Workbook is already closed\n");
    return Worksheet(workbook_add_worksheet(ptr, sheetname.data()));
}


Format Workbook::add_format()
{
    assert(ptr && "Workbook is already closed\n");
    return Format(workbook_add_format(ptr));
}


Chart Workbook::add_chart(const ChartType chart_type)
{
    assert(ptr && "Workbook is already closed\n");
    return Chart(workbook_add_chart(ptr, FROM_ENUM(chart_type)));
}


void Workbook::close()
{
    if (ptr) {
        LXW_CHECK(workbook_close(ptr));
        ptr = nullptr;
    }
}


Worksheets Workbook::worksheets()
{
    Worksheets sheets;
    lxw_worksheet *worksheet;
    LXW_FOREACH_WORKSHEET(worksheet, ptr) {
        sheets.emplace_back(Worksheet(worksheet));
    }
    return sheets;
}


Worksheet Workbook::get_worksheet_by_name(const std::string &name)
{
    assert(ptr && "Workbook is already closed\n");
    return Worksheet(workbook_get_worksheet_by_name(ptr, name.data()));
}


void Workbook::set_properties(const Properties &properties)
{
    assert(ptr && "Workbook is already closed\n");
    LXW_CHECK(workbook_set_properties(ptr, properties.ptr));
}


void Workbook::set_custom_property(const std::string &name,
    const std::string &value)
{
    LXW_CHECK(workbook_set_custom_property_string(ptr, name.data(), value.data()));
}


void Workbook::set_custom_property(const std::string &name,
    const double value)
{
    LXW_CHECK(workbook_set_custom_property_number(ptr, name.data(), value));
}


void Workbook::set_custom_property(const std::string &name,
    const int32_t value)
{
    LXW_CHECK(workbook_set_custom_property_integer(ptr, name.data(), value));
}


void Workbook::set_custom_property(const std::string &name,
    const bool value)
{
    LXW_CHECK(workbook_set_custom_property_boolean(ptr, name.data(), value));
}


// TODO: specialize
//workbook_set_custom_property_datetime
//workbook_define_name

}   /* xlsxwriter */
