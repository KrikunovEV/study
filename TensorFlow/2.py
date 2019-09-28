import tensorflow as tf
import sys

DIM = 3
USE_GPU = False
GPU_MEMORY_FRACTION = 1.0

graph = tf.Graph()
with graph.as_default():
    v1 = tf.placeholder(shape=[None, DIM], dtype=tf.float32, name='v1')
    v2 = tf.placeholder(shape=[None, DIM], dtype=tf.float32, name='v2')

    sub = tf.subtract(v1, v2)
    square = tf.square(sub)
    sum = tf.reduce_sum(square, axis=1)

    distance = tf.sqrt(sum)

if USE_GPU:
    gpu_options = tf.GPUOptions(per_process_gpu_memory_fraction=GPU_MEMORY_FRACTION)
    session_conf = tf.ConfigProto(allow_soft_placement=True,
                                  log_device_placement=True,
                                  gpu_options=gpu_options)
else:
    session_conf = tf.ConfigProto(device_count={'CPU': 1, 'GPU': 0},
                                  allow_soft_placement=True,
                                  log_device_placement=True)

session = tf.Session(graph=graph, config=session_conf)

data = {'v1': [[1, 2, 3], [1, 2, 3]],
        'v2': [[0, 2, 4], [0, 2, 4]]}

print_op = [tf.print("sub:\n", sub, output_stream=sys.stdout),
            tf.print("square:\n", square, output_stream=sys.stdout),
            tf.print("sum:\n", sum, output_stream=sys.stdout),
            tf.print("distance:\n", distance, output_stream=sys.stdout)]

with session.as_default():
    distance_output, _ = session.run([distance, print_op], feed_dict={v1: data['v1'],
                                                                      v2: data['v2']})
