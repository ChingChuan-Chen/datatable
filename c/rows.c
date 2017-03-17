#include "rows.h"
#include "datatable.h"


/**
 * Given a datatable and a slice spec `(start, count, step)`, constructs a
 * RowIndex object corresponding to applying this slice to the datatable.
 */
RowIndex* dt_select_row_slice(
    dt_DatatableObject *dt, int64_t start, int64_t count, int64_t step)
{
    RowIndex *res = malloc(sizeof(RowIndex));
    if (res == NULL) return NULL;

    if (dt->rowindex == NULL) {
        res->type = RI_SLICE;
        res->length = count;
        res->slice.start = start;
        res->slice.step = step;
    }
    else if (dt->rowindex->type == RI_SLICE) {
        int64_t srcstart = dt->rowindex->slice.start;
        int64_t srcstep = dt->rowindex->slice.step;
        res->type = RI_SLICE;
        res->length = count;
        res->slice.start = srcstart + srcstep * start;
        res->slice.step = step * srcstep;
    }
    else if (dt->rowindex->type == RI_ARRAY) {
        int64_t *data = malloc(sizeof(int64_t) * count);
        if (data == NULL) {
            free(res);
            return NULL;
        }
        int64_t *srcrows = dt->rowindex->indices;
        for (int64_t i = 0, isrc = start; i < count; i++, isrc += step) {
            data[i] = srcrows[isrc];
        }
        res->type = RI_ARRAY;
        res->length = count;
        res->indices = data;
    } else assert(0);

    return res;
}




RowIndex* dt_select_row_indices(
    dt_DatatableObject *dt, int64_t* data, int64_t len)
{
    RowIndex *res = malloc(sizeof(RowIndex));
    if (res == NULL) return NULL;

    if (dt->rowindex == NULL) {}
    else if (dt->rowindex->type == RI_SLICE) {
        int64_t srcstart = dt->rowindex->slice.start;
        int64_t srcstep = dt->rowindex->slice.step;
        for (int64_t i = 0; i < len; ++i) {
            data[i] = srcstart + data[i] * srcstep;
        }
    }
    else if (dt->rowindex->type == RI_ARRAY) {
        int64_t *srcrows = dt->rowindex->indices;
        for (int64_t i = 0; i < len; ++i) {
            data[i] = srcrows[data[i]];
        }
    } else assert(0);

    res->type = RI_ARRAY;
    res->length = len;
    res->indices = data;
    return res;
}



void dt_rowindex_dealloc(RowIndex *ri) {
    if (ri->type == RI_ARRAY) {
        free(ri->indices);
    }
}
