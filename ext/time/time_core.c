#include "ruby.h"
#include "ruby/encoding.h"

static VALUE cTime;

VALUE time__parse(VALUE str, VALUE comp);

static size_t
get_limit(VALUE opt)
{
    if (!NIL_P(opt)) {
        VALUE limit = rb_hash_aref(opt, ID2SYM(rb_intern("limit")));
        if (NIL_P(limit)) return SIZE_MAX;
        return NUM2SIZET(limit);
    }
    return 128;
}

static void
check_limit(VALUE str, VALUE opt)
{
    size_t slen, limit;
    if (NIL_P(str)) return;
    StringValue(str);
    slen = RSTRING_LEN(str);
    limit = get_limit(opt);
    if (slen > limit) {
        rb_raise(rb_eArgError,
                 "string length (%"PRI_SIZE_PREFIX"u) exceeds the limit %"PRI_SIZE_PREFIX"u", slen, limit);
    }
}

static VALUE
time_s__parse_internal(int argc, VALUE *argv, VALUE klass)
{
    VALUE vstr, vcomp, hash, opt;

    argc = rb_scan_args(argc, argv, "11:", &vstr, &vcomp, &opt);
    check_limit(vstr, opt);
    StringValue(vstr);
    if (!rb_enc_str_asciicompat_p(vstr))
        rb_raise(rb_eArgError,
                 "string should have ASCII compatible encoding");
    if (argc < 2)
        vcomp = Qtrue;

    hash = time__parse(vstr, vcomp);

    return hash;
}

static VALUE
time_s__parse(int argc, VALUE *argv, VALUE klass)
{
    return time_s__parse_internal(argc, argv, klass);
}

void
Init_time_core(void)
{
    cTime = rb_define_class("Time", rb_cObject);

    rb_define_singleton_method(cTime, "_parse", time_s__parse, -1);
}
