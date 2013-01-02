import unittest as unittest
import numpy as np
import rftk.assert_util
import rftk.buffers as buffers


class TestBuffers(unittest.TestCase):

    def test_img_float_buffer(self):
        A = np.array([[[3,21,1],[22,1,5]],[[2,3,4],[7,7,7]]], dtype=np.float32 )
        a = buffers.imgsBufferFloat( A )
        B = np.zeros((2,2,3), dtype=np.float32)
        a.AsNumpy(B)
        self.assertTrue((A == B).all())
        self.assertEqual(a.Get(1,0,2), 4)
        a.Set(1,0,2, 22)
        self.assertEqual(a.Get(1,0,2), 22)        

    def test_img_int_buffer(self):
        A = np.array([[[3,21,1],[22,1,5]],[[2,3,4],[7,7,7]]], dtype=np.int32 )
        a = buffers.imgsBufferInt( A )
        B = np.zeros((2,2,3), dtype=np.int32)
        a.AsNumpy(B)
        self.assertTrue((A == B).all())
        self.assertEqual(a.Get(1,0,2), 4)
        a.Set(1,0,2, 22)
        self.assertEqual(a.Get(1,0,2), 22) 

    def test_matrix_2d_float_buffer(self):
        A = np.array([[3,21,1],[22,33,5]], dtype=np.float32 )
        a = buffers.matrixBufferFloat( A )
        B = np.zeros((2,3), dtype=np.float32)
        a.AsNumpy(B)
        self.assertTrue((A == B).all())
        self.assertEqual(a.Get(1,2), 5)
        a.Set(0,2,22)
        self.assertEqual(a.Get(0,2), 22)        

    def test_matrix_2d_int_buffer(self):
        A = np.array([[3,21,1],[22,33,5]], dtype=np.int32 )
        a = buffers.matrixBufferInt( A )
        B = np.zeros((2,3), dtype=np.int32)
        a.AsNumpy(B)
        self.assertTrue((A == B).all())
        self.assertEqual(a.Get(1,2), 5)
        a.Set(0,2,22)
        self.assertEqual(a.Get(0,2), 22)   

    def test_matrix_1d_float_buffer(self):
        A = np.array([3,21,1], dtype=np.float32 )
        a = buffers.vecBufferFloat( A )
        B = np.zeros((3,1), dtype=np.float32)
        a.AsNumpy(B)
        self.assertTrue((A == B.flatten()).all())
        self.assertEqual(a.Get(2,0), 1)
        a.Set(2,0,22)
        self.assertEqual(a.Get(2,0), 22)        

    def test_matrix_1d_int_buffer(self):
        A = np.array([3,21,1], dtype=np.int32 )
        a = buffers.vecBufferInt( A )
        B = np.zeros((3,1), dtype=np.int32)
        a.AsNumpy(B)
        self.assertTrue((A == B.flatten()).all())
        self.assertEqual(a.Get(2,0), 1)
        a.Set(2,0,22)
        self.assertEqual(a.Get(2,0), 22)   

    def test_img_float_buffer_dim_exception(self):
        A = np.array([[[3,21,1],[22,1,5]],[[2,2,2],[7,7,7]]], dtype=np.float32 )
        a = buffers.imgsBufferFloat( A )
        B = np.zeros((2,3,3), dtype=np.float32)
        with self.assertRaises(TypeError):
            a.AsNumpy(B)

    def test_img_int_buffer_dim_exception(self):
        A = np.array([[[3,21,1],[22,1,5]],[[2,2,2],[7,7,7]]], dtype=np.int32 )
        a = buffers.imgsBufferInt( A )
        B = np.zeros((1,2,3), dtype=np.int32)
        with self.assertRaises(TypeError):
            a.AsNumpy(B)

    def test_matrix_float_buffer_dim_exception(self):
        A = np.array([[3,21,1],[22,33,5]], dtype=np.float32 )
        a = buffers.matrixBufferFloat( A )
        B = np.zeros((1,3), dtype=np.float32)
        with self.assertRaises(TypeError):
            a.AsNumpy(B)

    def test_matrix_int_buffer_dim_exception(self):
        A = np.array([[3,21,1],[22,33,5]], dtype=np.int32 )
        a = buffers.matrixBufferInt( A )
        B = np.zeros((2,1), dtype=np.int32)
        with self.assertRaises(TypeError):
            a.AsNumpy(B)

    def test_img_float_buffer_out_of_range_exception(self):
        A = np.array([[[3,21,1],[22,1,5]],[[2,2,2],[7,7,7]]], dtype=np.float32 )
        a = buffers.imgsBufferFloat( A )
        with self.assertRaises(IndexError):
            a.Get(2,0,0)
        with self.assertRaises(IndexError):
            a.Get(-1,0,0)
        with self.assertRaises(IndexError):
            a.Get(0,2,0)
        with self.assertRaises(IndexError):
            a.Get(0,-1,0)
        with self.assertRaises(IndexError):
            a.Get(0,0,3)
        with self.assertRaises(IndexError):
            a.Get(0,0,-1)

    def test_img_int_buffer_out_of_range_exception(self):
        A = np.array([[[3,21,1],[22,1,5]],[[2,2,2],[7,7,7]]], dtype=np.int32 )
        a = buffers.imgsBufferInt( A )
        with self.assertRaises(IndexError):
            a.Get(2,0,0)
        with self.assertRaises(IndexError):
            a.Get(-1,0,0)
        with self.assertRaises(IndexError):
            a.Get(0,2,0)
        with self.assertRaises(IndexError):
            a.Get(0,-1,0)
        with self.assertRaises(IndexError):
            a.Get(0,0,3)
        with self.assertRaises(IndexError):
            a.Get(0,0,-1)

    def test_matrix_float_buffer_out_of_range_exception(self):
        A = np.array([[3,21,1],[22,33,5]], dtype=np.float32 )
        a = buffers.matrixBufferFloat( A )
        with self.assertRaises(IndexError):
            a.Get(2,0)
        with self.assertRaises(IndexError):
            a.Get(-1,0)
        with self.assertRaises(IndexError):
            a.Get(0,3)
        with self.assertRaises(IndexError):
            a.Get(0,-1)

    def test_matrix_int_buffer_out_of_range_exception(self):
        A = np.array([[3,21,1],[22,33,5]], dtype=np.int32 )
        a = buffers.matrixBufferInt( A )
        with self.assertRaises(IndexError):
            a.Get(2,0)
        with self.assertRaises(IndexError):
            a.Get(-1,0)
        with self.assertRaises(IndexError):
            a.Get(0,3)
        with self.assertRaises(IndexError):
            a.Get(0,-1)

    def test_buffer_shared_memory_copy(self):
        data_float = np.arange(1000000).reshape(1000,1000)
        data_int = np.array( data_float, dtype=np.int32)

        matrix_buffer_float = buffers.matrixBufferFloat64( data_float )
        matrix_buffer_int = buffers.matrixBufferInt( data_int )
        img_buffer_float = buffers.imgBufferFloat64( data_float )
        img_buffer_int = buffers.imgBufferFloat64( data_int )

        # Check that changing a memory copy changes the original
        copy = matrix_buffer_float.SharedMemoryCopy()
        copy.Set(100, 100, -22)
        self.assertEqual( matrix_buffer_float.Get(100,100), -22 )

        copy = matrix_buffer_int.SharedMemoryCopy()
        copy.Set(100, 100, -22)
        self.assertEqual( matrix_buffer_int.Get(100,100), -22 )

        copy = img_buffer_float.SharedMemoryCopy()
        copy.Set(0, 100, 100, -22)
        self.assertEqual( img_buffer_float.Get(0, 100,100), -22 )

        copy = img_buffer_int.SharedMemoryCopy()
        copy.Set(0, 100, 100, -22)
        self.assertEqual( img_buffer_int.Get(0, 100,100), -22 )

        # The loop below just checks than very little memory is used per copy
        list_of_ref = []
        for i in range(10000):
            list_of_ref.append( matrix_buffer_float.SharedMemoryCopy() )
            list_of_ref.append( matrix_buffer_int.SharedMemoryCopy() )
            list_of_ref.append( img_buffer_float.SharedMemoryCopy() )
            list_of_ref.append( img_buffer_int.SharedMemoryCopy() )



if __name__ == '__main__':
    unittest.main()