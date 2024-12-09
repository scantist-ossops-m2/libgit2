#include "clar_libgit2.h"
#include "odb.h"
#include "pack_data_one.h"
#include "pack.h"

static git_odb *_odb;

void test_odb_packed_one__initialize(void)
{
	git_odb_backend *backend = NULL;

	cl_git_pass(git_odb_new(&_odb));
	cl_git_pass(git_odb_backend_one_pack(&backend, cl_fixture("testrepo.git/objects/pack/pack-a81e489679b7d3418f9ab594bda8ceb37dd4c695.idx")));
	cl_git_pass(git_odb_add_backend(_odb, backend, 1));
}

void test_odb_packed_one__cleanup(void)
{
	git_odb_free(_odb);
	_odb = NULL;
}

void test_odb_packed_one__mass_read(void)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(packed_objects_one); ++i) {
		git_oid id;
		git_odb_object *obj;

		cl_git_pass(git_oid_fromstr(&id, packed_objects_one[i]));
		cl_assert(git_odb_exists(_odb, &id) == 1);
		cl_git_pass(git_odb_read(&obj, _odb, &id));

		git_odb_object_free(obj);
	}
}

void test_odb_packed_one__read_header_0(void)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(packed_objects_one); ++i) {
		git_oid id;
		git_odb_object *obj;
		size_t len;
		git_otype type;

		cl_git_pass(git_oid_fromstr(&id, packed_objects_one[i]));

		cl_git_pass(git_odb_read(&obj, _odb, &id));
		cl_git_pass(git_odb_read_header(&len, &type, _odb, &id));

		cl_assert(obj->raw.len == len);
		cl_assert(obj->raw.type == type);

		git_odb_object_free(obj);
	}
}