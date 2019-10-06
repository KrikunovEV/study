import tensorflow as tf
import sys


tf.enable_eager_execution()

tensor = tf.range(10)
tf.print("tensor:", tensor, output_stream=sys.stdout)

tensor = tf.square(tensor)
tf.print("tensor:", tensor, output_stream=sys.stdout)

tensor = tf.range(10)
print(tensor)
