import tensorflow as tf

USE_GPU = False
GPU_MEMORY_FRACTION = 1.0
DIM = 3

graph = tf.Graph()
with graph.as_default():
    v1 = tf.placeholder(shape=[DIM], dtype=tf.float32, name='vector1')
    v2 = tf.placeholder(shape=[DIM], dtype=tf.float32, name='vector2')

    sub = tf.subtract(v1, v2)
    square = tf.square(sub)
    sum = tf.reduce_sum(square)

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

data = {'v1': [1, 2, 3], 'v2': [0, 2, 4]}

with session.as_default():
    distance_output = session.run(distance, feed_dict={v1: data['v1'],
                                                       v2: data['v2']})
print(distance_output)