import tensorflow as tf

priority = tf.placeholder(shape=(None), dtype=tf.int64)
image_path = tf.placeholder(shape=(None), dtype=tf.string)
image_label = tf.placeholder(shape=(None), dtype=tf.int64)

queue = tf.PriorityQueue(capacity=10, types=[tf.string, tf.int64], shapes=[(), ()])

enqueue_op = queue.enqueue_many([priority, image_path, image_label])