#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "linalg_obj.h"
#include "vector.h"
#include "errors.c"
#include "util.c"

struct vector* vector_new(int length) {
    struct vector* new_vector = malloc(sizeof(struct vector));
    check_memory((void*)new_vector);

    DATA(new_vector) = malloc((sizeof(double))*length);
    check_memory((void*) DATA(new_vector));

    new_vector->length = length;
    OWNS_MEMORY(new_vector)= true;
    MEMORY_OWNER(new_vector) = NULL;
    REF_COUNT(new_vector) = 0;

    return new_vector;
}

struct vector* vector_new_view(struct linalg_obj* parent, double* view, int length) {
    struct vector* new_vector = malloc(sizeof(struct vector));
    check_memory((void*)new_vector);

    DATA(new_vector) = view;
    new_vector->length = length;
    OWNS_MEMORY(new_vector) = false;
    MEMORY_OWNER(new_vector) = parent;
    REF_COUNT(new_vector) = 0;
    REF_COUNT(parent) += 1;

    return new_vector;
}

void vector_free(struct vector* v) {
    struct linalg_obj* mem_owner;
    if(OWNS_MEMORY(v)) {
        if(REF_COUNT(v)== 0) {
            free(DATA(v));
            free(v);
        } else {
            raise_non_zero_reference_free_error();
        }
    } else {
        if(REF_COUNT(v) == 0) {
            mem_owner = MEMORY_OWNER(v);
            REF_COUNT(mem_owner) -= 1;
            free(v);
        } else {
            raise_non_zero_reference_free_error();
        }
    }
}

struct vector* vector_zeros(int length) {
    struct vector* v = vector_new(length);
    for(int i = 0; i < v->length; i++) {
        DATA(v)[i] = 0;
    }
    return v;
}

//struct vector* vector_linspace(int length, double min, double max) {
//    struct vector* v = vector_new(length);
//    double step = (max - min) / (length - 1);
//    for(int i = 0; i < v->length; i++) {
//        v->data[i] = min + step*i;
//    }
//    return v;
//}
//
//struct vector* vector_from_array(int length, double *data) {
//    struct vector* v = vector_new(length);
//    for(int i = 0; i < v->length; i++) {
//        v->data[i] = data[i];
//    }
//    return v;
//}
//
//struct vector* vector_slice(int begin_idx, int end_idx, struct vector* v) {
//    int new_vector_length = end_idx - begin_idx;
//    double* begin_ptr = v->data + begin_idx;
//    struct vector* w = vector_new_view(new_vector_length, begin_ptr, v);
//    return w;
//}
//
//struct vector* vector_subtract(struct vector* v1, struct vector* v2) {
//    _vector_check_lengths(v1, v2);
//    struct vector* v = vector_new(v1->length);
//    for(int i = 0; i < v->length; i++) {
//        v->data[i] = v1->data[i] - v2->data[i];
//    }
//    return v;
//}
//
//void vector_subtract_into(struct vector* v1, struct vector* v2) {
//    _vector_check_lengths(v1, v2);
//    for(int i = 0; i < v1->length; i++) {
//        v1->data[i] = v1->data[i] - v2->data[i];
//    }
//}
//
//struct vector* vector_add(struct vector* v1, struct vector* v2) {
//    _vector_check_lengths(v1, v2);
//    struct vector* v = vector_new(v1->length);
//    for(int i = 0; i < v->length; i++) {
//        v->data[i] = v1->data[i] + v2->data[i];
//    }
//    return v;
//}
//
//void vector_add_into(struct vector* v1, struct vector* v2) {
//    _vector_check_lengths(v1, v2);
//    for(int i = 0; i < v1->length; i++) {
//        v1->data[i] = v1->data[i] + v2->data[i];
//    }
//}
//
//double vector_dot_product(struct vector* v1, struct vector* v2) {
//    _vector_check_lengths(v1, v2);
//    double dp = 0;
//    for(int i = 0; i < v1->length; i++) {
//        dp += v1->data[i] * v2->data[i];
//    }
//    return dp;
//}

void vector_print(struct vector* v) {
    if(v->length == 0) {
        printf("[]\n");
    } else if (v->length == 1) {
        printf("[%.2f]\n", DATA(v)[0]);
    } else {
        printf("[%.2f", DATA(v)[0]);
        for(int i = 1; i < v->length - 1; i++) {
            printf(", ");
            printf("%.2f", DATA(v)[i]);
        }
        printf(", %.2f]\n", DATA(v)[v->length - 1]);
    }
}

void _vector_check_lengths(struct vector* v1, struct vector* v2) {
    if(v1->length != v2->length) {
        raise_non_commensurate_vector_error();
    }
}
